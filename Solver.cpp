#include <unordered_set>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include "Solver.h"

Solver::~Solver() {
}

void Solver::initiateRandomSolution() {
    int meetingsPerTimeslot = mProblem.mTeams.size() / 2;
    tSchedule schedule(mProblem.mSlots.size(), tMeetings(meetingsPerTimeslot));

    auto teams = mProblem.mTeams;

    int numTeams = teams.size();
    int rounds = (teams.size() - 1) * 2;

    for (int round = 0; round < rounds; ++round) {
        for (int game = 0; game < meetingsPerTimeslot; ++game) {
            int team1 = teams[game];
            int team2 = teams[numTeams - 1 - game];


            if (round < teams.size() - 1) {
                Meeting temp = {team1, team2};
                schedule[round][game] = temp;
            } else {
                Meeting temp = {team2, team1};
                schedule[round][game] = temp;
            }

        }
        std::rotate(teams.begin() + 1, teams.begin() + numTeams - 1, teams.end());
    }

    Solution solution;

    std::mt19937 mt(mSeed);
    std::shuffle(schedule.begin(), schedule.begin() + rounds / 2, mt);
    std::shuffle(schedule.begin() + rounds / 2, schedule.end(), mt);

    solution.setMSchedule(schedule);
    evaluate(solution);
    mSolutions.push_back(solution);
}

bool AreSchedulesEqual(const tSchedule &schedule1, const tSchedule &schedule2) {
    for (size_t i = 0; i < schedule1.size(); ++i) {
        const tMeetings &round1 = schedule1[i];
        const tMeetings &round2 = schedule2[i];

        for (size_t j = 0; j < round1.size(); ++j) {
            const Meeting &meeting1 = round1[j];
            const Meeting &meeting2 = round2[j];

            if (meeting1.home != meeting2.home || meeting1.away != meeting2.away) {
                return false;
            }
        }
    }

    return true;
}

//PARTIAL SWAP ROUNDS
bool RefineTeamsSet(Solution &solution, int roundIndex, std::set<int> &Ts) {
    bool any_change = false;
    bool changed;
    do {
        changed = false;
        std::set<int> newTs;

        for (int team: Ts) {
            bool validTeam = true;
            for (const auto &meeting: solution.mSchedule[roundIndex]) {
                if ((meeting.home == team && Ts.find(meeting.away) == Ts.end()) ||
                    (meeting.away == team && Ts.find(meeting.home) == Ts.end())) {
                    validTeam = false;
                    break;
                }
            }
            if (validTeam) {
                newTs.insert(team);
            } else {
                changed = true;
                any_change = true;
            }
        }

        Ts = newTs;
    } while (changed && !Ts.empty());
    return any_change;
}

void Solver::partialSwapRounds(Solution &solution) {
    std::uniform_int_distribution<> distRounds(0, mProblem.mSlots.size() - 1);
    std::uniform_int_distribution<> distTeamsCount(2, (mProblem.mTeams.size() - 1));
    std::uniform_int_distribution<> distTeams(0, (mProblem.mTeams.size() - 1));

    Solution s;
    s.copyMSchedule(solution.mSchedule);

    //Losowanie
    int ri, rj;
    do {
        ri = distRounds(gen);
        rj = distRounds(gen);
    } while (ri == rj);
    int teamsNumber = distTeamsCount(gen);
    std::set<int> Ts;
    while (Ts.size() < teamsNumber) {
        int tempTeam = distTeams(gen);
        Ts.insert(tempTeam);
    }


    bool changed;
    do {
        RefineTeamsSet(solution, ri, Ts);
        if (Ts.empty()) {
            break;
        }
        changed = RefineTeamsSet(solution, rj, Ts);
    } while (changed);
    if (!Ts.empty()) {


        std::vector<Meeting *> meetingsToSwapRi;
        for (auto &meeting: solution.mSchedule[ri]) {
            if (Ts.find(meeting.home) != Ts.end() || Ts.find(meeting.away) != Ts.end()) {
                meetingsToSwapRi.push_back(&meeting);
            }
        }

        // Znajdź mecze do zamiany w rundzie rj
        std::vector<Meeting *> meetingsToSwapRj;
        for (auto &meeting: solution.mSchedule[rj]) {
            if (Ts.find(meeting.home) != Ts.end() || Ts.find(meeting.away) != Ts.end()) {
                meetingsToSwapRj.push_back(&meeting);
            }
        }


        // Zamień mecze
        if (meetingsToSwapRi.size() == meetingsToSwapRj.size()) {
            for (size_t i = 0; i < meetingsToSwapRi.size(); ++i) {
                std::swap(*meetingsToSwapRi[i], *meetingsToSwapRj[i]);
            }
            auto temp = AreSchedulesEqual(s.mSchedule, solution.mSchedule);
        }
    }
}

//PARTIAL SWAP TEAMS
bool IsValidSwap(const Solution &solution, const std::set<int> &Rs, int ti, int tj) {
    std::map<int, int> countVsTi, countVsTj;

    for (int roundIndex: Rs) {
        for (const auto &meeting: solution.mSchedule[roundIndex]) {
            if (meeting.home == ti || meeting.away == ti) {
                int otherTeam = (meeting.home == ti) ? meeting.away : meeting.home;
                countVsTi[otherTeam]++;
            }
            if (meeting.home == tj || meeting.away == tj) {
                int otherTeam = (meeting.home == tj) ? meeting.away : meeting.home;
                countVsTj[otherTeam]++;
            }
        }
    }

    for (const auto &pair: countVsTi) {
        if (countVsTj[pair.first] != pair.second) {
            return false;
        }
    }

    return true;
}

void Solver::partialSwapTeams(Solution &solution, bool phased) {

    int roundBorderValue = mProblem.mSlots.size() - 1;

    if (phased) {
        roundBorderValue = mProblem.mSlots.size() / 2 - 2;
    }

    std::uniform_int_distribution<> distRounds(0, roundBorderValue);
    std::uniform_int_distribution<> distRoundsCount(2, roundBorderValue);
    std::uniform_int_distribution<> distTeams(0, (mProblem.mTeams.size() - 1));

    Solution s;
    s.copyMSchedule(solution.mSchedule);

    //Losowanie
    int ti, tj;
    do {
        ti = distTeams(gen);
        tj = distTeams(gen);
    } while (ti == tj);
    int roundsNumber = distRoundsCount(gen);
    std::set<int> Rs;
    while (Rs.size() < roundsNumber) {
        int tempTeam = distRounds(gen);
        Rs.insert(tempTeam);
    }


    if (ti == tj || !IsValidSwap(solution, Rs, ti, tj)) {
        return;
    }

    for (int roundIndex: Rs) {
        for (auto &meeting: solution.mSchedule[roundIndex]) {
            if (meeting.home == ti) {
                meeting.home = tj;
            } else if (meeting.home == tj) {
                meeting.home = ti;
            }
            if (meeting.away == ti) {
                meeting.away = tj;
            } else if (meeting.away == tj) {
                meeting.away = ti;
            }
        }
    }

    auto temp = mProblem.mConstraints[0]->isViolated(solution);
    auto temp1 = AreSchedulesEqual(s.mSchedule, solution.mSchedule);
    auto temp2 = 3;
}


void Solver::partialSwapTeamsPhased(Solution &solution) {

}


void Solver::swapRounds(Solution &solution) {
    std::uniform_int_distribution<> dist(0, mProblem.mSlots.size() - 1);
    int roundA, roundB;
    do {
        roundA = dist(gen);
        roundB = dist(gen);
    } while (roundA == roundB);

    auto temp = solution.mSchedule[roundA];
    solution.mSchedule[roundA] = solution.mSchedule[roundB];
    solution.mSchedule[roundB] = temp;
}

void Solver::swapTeams(Solution &solution) {
    std::uniform_int_distribution<> dist(0, mProblem.mTeams.size() - 1);
    int teamA, teamB;
    do {
        teamA = dist(gen);
        teamB = dist(gen);
    } while (teamA == teamB);

    for (int round = 0; round < solution.mSchedule.size(); round++) {
        for (int meeting = 0; meeting < solution.mSchedule[round].size(); meeting++)
            solution.mSchedule[round][meeting].swapTeams(teamA, teamB);
    }
}

void Solver::swapHomes(Solution &solution) {
    std::uniform_int_distribution<> dist(0, mProblem.mTeams.size() - 1);
    int teamA, teamB;
    do {
        teamA = dist(gen);
        teamB = dist(gen);
    } while (teamA == teamB);

    for (int round = 0; round < solution.mSchedule.size(); round++) {
        for (int meeting = 0; meeting < solution.mSchedule[round].size(); meeting++)
            if (solution.mSchedule[round][meeting].isPartiallyEqual({teamA, teamB})) {
                solution.mSchedule[round][meeting].swapHomeAway();
            }
    }
}

void Solver::anneal() {
    int count_neighbours = 5;
    int number_of_definitions = 6;
    int sum_of_neighbours = count_neighbours * number_of_definitions;

    initiateRandomSolution();

    Solution bestGlobalSolution;
    bestGlobalSolution.setMSchedule(mSolutions[0].mSchedule);
    bestGlobalSolution.setMFitness(mSolutions[0].mFitness);
    bestGlobalSolution.setMHardViolation(mSolutions[0].mHardViolation);

    Solution currSolution;
    currSolution.setMSchedule(mSolutions[0].mSchedule);
    currSolution.setMFitness(mSolutions[0].mFitness);
    currSolution.setMHardViolation(mSolutions[0].mHardViolation);

    Solution bestNew;
    unsigned int allCounter = 0;


    auto temperature = mProblem.mParams.tStart;
    while (temperature > mProblem.mParams.tMin) {

        std::vector<Solution> neighboursVector;

        int innerLoopCounter = 0;
        while (innerLoopCounter < mProblem.mParams.innerLoop) {
            //Tworzę wektor z wieloma kopiami aktualnego rozwiązania
            for (int i = 0; i < sum_of_neighbours; i++) {
                Solution temp;
                temp.setMSchedule(currSolution.mSchedule);
                neighboursVector.push_back(temp);
            }

            //Generuję sąsiedztwa
            for (int i = 0; i < count_neighbours; i++) {
                swapTeams(neighboursVector[i]);
                swapHomes(neighboursVector[i + count_neighbours]);
                swapRounds(neighboursVector[i + count_neighbours * 2]);
                partialSwapRounds(neighboursVector[i + count_neighbours * 3]);
                partialSwapTeams(neighboursVector[i + count_neighbours * 4], false);
                partialSwapTeams(neighboursVector[i + count_neighbours * 5], true);
            }

            //Ewaluuję każde sąsiedztwo
            for (int i = 0; i < neighboursVector.size(); i++) {
                evaluate(neighboursVector[i]);
            }


            //Wybieram najlepsze z nowych rozwiązań.
            int neighID = 0;
            int bestID = 0;
            bestNew.setMSchedule(neighboursVector[neighID].mSchedule);
            bestNew.setMFitness(neighboursVector[neighID].mFitness);
            bestNew.setMHardViolation(neighboursVector[neighID].mHardViolation);


            for (int i = 0; i < neighboursVector.size(); i++) {
                if (neighboursVector[i].mFitness < bestNew.mFitness) {
                    bestNew.setMSchedule(neighboursVector[i].mSchedule);
                    bestNew.setMFitness(neighboursVector[i].mFitness);
                    bestNew.setMHardViolation(neighboursVector[i].mHardViolation);
                    bestID = i;
                }
            }

            //Jeśli nowe najlepsze, jest lepsze niż aktualne, akceptuję je jako currSolution
            if (bestNew.mFitness < currSolution.mFitness) {
                currSolution.setMSchedule(bestNew.mSchedule);
                currSolution.setMFitness(bestNew.mFitness);
                currSolution.setMHardViolation(bestNew.mHardViolation);

                //Przy okazji sprawdzam, czy jest najlepszym globalnym rozwiązaniem
                if (currSolution.mFitness < bestGlobalSolution.mFitness) {
                    bestGlobalSolution.setMSchedule(currSolution.mSchedule);
                    bestGlobalSolution.setMFitness(currSolution.mFitness);
                    bestGlobalSolution.setMHardViolation(currSolution.mHardViolation);
                }
            }
                //w przeciwnym wypadku, akceptuję je jako nowe z pewną dozą prawdopodobieństwa
            else {
                double randomNumber = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                double valueToChangeSolution = exp(
                        -((bestNew.mFitness - currSolution.mFitness) / temperature));
                if (randomNumber < valueToChangeSolution) {
                    currSolution.setMSchedule(bestNew.mSchedule);
                    currSolution.setMFitness(bestNew.mFitness);
                    currSolution.setMHardViolation(bestNew.mHardViolation);
                }
            }
            allCounter++;
            innerLoopCounter++;
            std::cout << bestGlobalSolution.mFitness << std::endl;

            //Zwalniam pamięć
            neighboursVector.clear();

        }
        temperature = temperature * mProblem.mParams.coolingRate;
    }
    mSolutions[0].setMSchedule(bestGlobalSolution.mSchedule);
    mSolutions[0].setMFitness(bestGlobalSolution.mFitness);
    mSolutions[0].setMHardViolation(bestGlobalSolution.mHardViolation);
}

void Solver::evaluate(Solution &solution) {
    for (auto constraint: mProblem.mConstraints) {
        constraint->isViolated(solution);
    }
}
