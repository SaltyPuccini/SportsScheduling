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

    std::shuffle(schedule.begin(), schedule.begin() + rounds / 2, gen);
    std::shuffle(schedule.begin() + rounds / 2, schedule.end(), gen);

    solution.setMSchedule(schedule);
    evaluate(solution);
    mSolution = solution;
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

        // find meetings to swap in round rj
        std::vector<Meeting *> meetingsToSwapRj;
        for (auto &meeting: solution.mSchedule[rj]) {
            if (Ts.find(meeting.home) != Ts.end() || Ts.find(meeting.away) != Ts.end()) {
                meetingsToSwapRj.push_back(&meeting);
            }
        }


        // Swap meetings
        if (meetingsToSwapRi.size() == meetingsToSwapRj.size()) {
            for (size_t i = 0; i < meetingsToSwapRi.size(); ++i) {
                std::swap(*meetingsToSwapRi[i], *meetingsToSwapRj[i]);
            }
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
    unsigned int allCounter = 0;
    auto neighbourhoods = {Home, Rounds, Teams, PRounds, PTeams, PartialTeamsP};


    std::map<NeighbourhoodType, int> neighbourhoodSize;

    for (const auto &neighbourhood: neighbourhoods) {
        neighbourhoodSize[neighbourhood] = mProblem.mParams.neighbourhoodSize;
    }

    initiateRandomSolution();

    Solution bestGlobalSolution;
    bestGlobalSolution.setMSchedule(mSolution.mSchedule);
    bestGlobalSolution.setMFitness(mSolution.mFitness);

    Solution currSolution;
    currSolution.setMSchedule(mSolution.mSchedule);
    currSolution.setMFitness(mSolution.mFitness);

    Solution bestNew;


    auto temperature = mProblem.mParams.tStart;
    while (allCounter < mProblem.mParams.iterations) {

        //Archiving the current solution
        mCurr.push_back(currSolution.mFitness);
        mTemperatureArchive.push_back(temperature);



        //Create a vector with copies of the current solution
        std::vector<std::pair<NeighbourhoodType, Solution>> neighboursVector;
        for (const auto &neighbourhood: neighbourhoods) {
            for (int i = 0; i < neighbourhoodSize[neighbourhood]; ++i) {
                Solution sol;
                sol.setMSchedule(currSolution.mSchedule);
                neighboursVector.push_back({neighbourhood, sol});
            }
        }

        //Generate neighborhoods and evaluate them
        for (auto &pair: neighboursVector) {
            switch (pair.first) {
                case Home:
                    swapHomes(pair.second);
                    break;
                case Rounds:
                    swapRounds(pair.second);
                    break;
                case Teams:
                    swapTeams(pair.second);
                    break;
                case PRounds:
                    partialSwapRounds(pair.second);
                    break;
                case PTeams:
                    partialSwapTeams(pair.second, false);
                    break;
                case PartialTeamsP:
                    partialSwapTeams(pair.second, true);
                    break;
                default:

                    break;
            }
            evaluate(pair.second);
        }

        //Sort them by highest fitness level
        std::sort(neighboursVector.begin(), neighboursVector.end(),
                  [](std::pair<NeighbourhoodType, Solution> a, std::pair<NeighbourhoodType, Solution> b) {
                      return a.second.mFitness < b.second.mFitness;
                  });

        int size = neighbourhoodSize.size() * mProblem.mParams.neighbourhoodSize;
        for (auto &pair: neighbourhoodSize) {
            pair.second = 1;
            size--;
        }

        //Measuring performance
        if (mProblem.mParams.isAdaptive) {
            std::map<NeighbourhoodType, int> bestPositions;

            for (int i = 0; i < neighboursVector.size(); ++i) {
                NeighbourhoodType currentType = neighboursVector[i].first;

                //If the neighborhood type has not yet been added to the map, add it with the current position
                if (bestPositions.find(currentType) == bestPositions.end()) {
                    bestPositions[currentType] = i;
                }
            }

            std::vector<std::pair<NeighbourhoodType, int>> sortedBestPositions;
            for (const auto &item: bestPositions) {
                sortedBestPositions.push_back(item);
            }
            std::sort(sortedBestPositions.begin(), sortedBestPositions.end(),
                      [](const std::pair<NeighbourhoodType, int> &a, const std::pair<NeighbourhoodType, int> &b) {
                          return a.second < b.second;
                      }
            );

            for (auto &pair: sortedBestPositions) {
                if (size > 2)
                    neighbourhoodSize[pair.first] += ceil(size / 2.0);
                size = size - ceil(size / 2.0);
            }
            int totalSize = 0;
            for (auto &pair: neighbourhoodSize) {
                totalSize += pair.second;
            }

            if (neighbourhoodSize.size() * mProblem.mParams.neighbourhoodSize - totalSize > 0) {
                neighbourhoodSize[sortedBestPositions[0].first] +=
                        neighbourhoodSize.size() * mProblem.mParams.neighbourhoodSize - totalSize;
            }
        } else if (!mProblem.mParams.isBasic) {
            for (auto pair: neighbourhoodSize) {
                std::uniform_int_distribution<> dist(0, size);
                auto generatedNumber = dist(gen);
                neighbourhoodSize[pair.first] += generatedNumber;
                size = size - generatedNumber;
            }
            if (neighbourhoodSize.size() * mProblem.mParams.neighbourhoodSize - size > 0) {
                neighbourhoodSize[begin(neighbourhoodSize)->first] += size;
            }
        }else{
            for (const auto &neighbourhood: neighbourhoods) {
                neighbourhoodSize[neighbourhood] = mProblem.mParams.neighbourhoodSize;
            }
        }

        //Choose the best of new solutions..
        bestNew.setMSchedule(neighboursVector.front().second.mSchedule);
        bestNew.setMFitness(neighboursVector.front().second.mFitness);

        //Archive the best, worst and average.
        float sumFitness = 0.0f;
        for (const auto &pair: neighboursVector) {
            sumFitness += pair.second.mFitness;
        }
        float averageFitness = sumFitness / neighboursVector.size();

        mBestFromNew.push_back(neighboursVector.front().second.mFitness);
        mWorstFromNew.push_back(neighboursVector.back().second.mFitness);
        mAvgFromNew.push_back(averageFitness);
        mBestNewNeighbourhood.push_back(neighboursVector.front().first);


        //If the new best is better than the current one, I accept it as a currSolution
        if (bestNew.mFitness < currSolution.mFitness) {
            auto a = 1;
            currSolution.setMSchedule(bestNew.mSchedule);
            currSolution.setMFitness(bestNew.mFitness);

            //I check whether it is the best global solution
            if (currSolution.mFitness < bestGlobalSolution.mFitness) {
                bestGlobalSolution.setMSchedule(currSolution.mSchedule);
                bestGlobalSolution.setMFitness(currSolution.mFitness);
            }
        }
            //Otherwise, I accept them as new with some degree of probability
        else {
            double randomNumber = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            double valueToChangeSolution = exp(
                    -((bestNew.mFitness - currSolution.mFitness) / temperature));
            if (randomNumber < valueToChangeSolution) {
                currSolution.setMSchedule(bestNew.mSchedule);
                currSolution.setMFitness(bestNew.mFitness);
            }
        }

        allCounter++;

        //Archive the best solution globally.
        mBest.push_back(bestGlobalSolution.mFitness);

        //Freeing memory
        neighboursVector.clear();

        //Reduce the temperature
        temperature = temperature * mProblem.mParams.coolingRate;
    }
    //The best found solution is saved as the mSolution
    mSolution.setMSchedule(bestGlobalSolution.mSchedule);
    mSolution.setMFitness(bestGlobalSolution.mFitness);
}


void Solver::evaluate(Solution &solution) {
    for (auto constraint: mProblem.mConstraints) {
        constraint->isViolated(solution);
    }
}

void Solver::initiateGlobalRNG(int seed) {
    mSeed = seed;
    gen.seed(mSeed);
}

void Solver::clearArchive() {
    mBest.clear();
    mTemperatureArchive.clear();
    mCurr.clear();
    mBestFromNew.clear();
    mAvgFromNew.clear();
    mWorstFromNew.clear();
}

int Solver::countSoftViolations() {
    auto numberOfViolations = 0;
    for (auto constraint: mProblem.mConstraints) {
        auto violationCondition = constraint->isViolated(mSolution);
        if (violationCondition) {
            if (!constraint->isHard()) {
                numberOfViolations++;
            }
        }
    }
    return numberOfViolations;
}

int Solver::countHardViolations() {
    auto numberOfViolations = 0;
    for (auto constraint: mProblem.mConstraints) {
        auto violationCondition = constraint->isViolated(mSolution);
        if (violationCondition) {
            if (constraint->isHard()) {
                numberOfViolations++;
            }
        }
    }
    return numberOfViolations;
}

