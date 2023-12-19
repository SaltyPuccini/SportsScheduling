#include <unordered_set>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <iostream>
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


bool areDuplicatesInVector(std::vector<int> vector) {
    std::unordered_map<int, int> map;
    for (size_t i = 0; i < vector.size(); ++i) {
        if (vector[i] != -1) {
            if (map.find(vector[i]) != map.end()) {
                return map[vector[i]];
            } else {
                map[vector[i]] = i;
            }
        }
    }
    return -1;
}

bool containsValue(const std::vector<int> &numbers, int valueToFind) {
    for (int number: numbers) {
        if (number == valueToFind) {
            return true;
        }
    }
    return false;
}

void Solver::partialSwapRounds(Solution &solution) {
    std::uniform_int_distribution<> distRounds(0, mProblem.mSlots.size() - 1);
    std::uniform_int_distribution<> distTeams(0, mProblem.mTeams.size() - 1);
    int roundA = distRounds(gen);
    int roundB = distRounds(gen);

    //Each team from teams must play only with other teams from teams.


    int roundsNumber = distTeams(gen);
    std::vector<int> teams;
    std::vector<int> resultingTeams;
    int tempTeam;

    for (int i = 0; i < roundsNumber; i++) {
        do {
            tempTeam = distTeams(gen);
        } while (std::find(teams.begin(), teams.end(), tempTeam) != teams.end());
        teams.push_back(tempTeam);
    }


    //Each team from teams must play only with other teams from teams.
    std::vector<std::vector<int>> enemies(mProblem.mTeams.size(), std::vector<int>(2, -1));

    for (auto team: teams) {
        for (auto meeting: solution.mSchedule[roundA]) {
            if (meeting.meetingContains(team)) {
                enemies[team][0] = meeting.whoIsEnemy(team);
            }
        }
        for (auto meeting: solution.mSchedule[roundB]) {
            if (meeting.meetingContains(team)) {
                enemies[team][1] = meeting.whoIsEnemy(team);
            }
        }
    }


    //prejrzec wszystkie teamy, czy ich przeciwnicy są w teams;
    //jesli jest ktos, bez wszystkich przeciwnikow w vektorze teams, to trzeba go wyrzucic
    //jak wyrzucimy teamek z vectora teams, trzeba zrobić przegląd od początku.

    int i = 0;
    while (i < teams.size()) {
        if (!containsValue(teams, enemies[teams[i]][0]) || !containsValue(teams, enemies[teams[i]][1])) {
            teams.erase(teams.begin() + i);
            i = 0;
        } else {
            i++;
        }
    }




    for (auto team: teams) {
        for (int meetingA = 0; meetingA < solution.mSchedule[roundA].size(); meetingA++) {
            if (solution.mSchedule[roundA][meetingA].meetingContains(team)) {
                for (int meetingB = 0; meetingB < solution.mSchedule[roundB].size(); meetingB++) {
                    if (solution.mSchedule[roundB][meetingB].meetingContains(team)) {
                        auto temp = solution.mSchedule[roundA][meetingA];
                        solution.mSchedule[roundA][meetingA] = solution.mSchedule[roundB][meetingB];
                        solution.mSchedule[roundB][meetingB] = temp;
                    }
                }
            }
        }
    }


}

void Solver::partialSwapTeams(Solution &solution) {
    std::mt19937 gen(mSeed);
    std::uniform_int_distribution<> distTeams(0, mProblem.mTeams.size() - 1);
    std::uniform_int_distribution<> distRounds(0, mProblem.mSlots.size() - 1);

    int teamA = distTeams(gen);
    int teamB = distTeams(gen);

    int roundsNumber = distRounds(gen);
    std::vector<int> rounds;
    int tempRound;

    for (int i = 0; i < roundsNumber; i++) {
        do {
            tempRound = distRounds(gen);
        } while (std::find(rounds.begin(), rounds.end(), tempRound) != rounds.end());
        rounds.push_back(tempRound);
    }

    std::vector<int> enemiesA(mProblem.mSlots.size(), -1);
    std::vector<int> enemiesB(mProblem.mSlots.size(), -1);


    //Znajduję przeciwników teamuA i teamuB
    for (int round = 0; round < rounds.size(); round++) {
        for (auto meeting: solution.mSchedule[round]) {
            if (meeting.meetingContains(teamA)) {
                enemiesA[round] = meeting.whoIsEnemy(teamA);
                enemiesB[round] = meeting.whoIsEnemy(teamB);

            }
        }
    }

    //Z rund usuwam te, wśród których są duplikaty przeciwników teamuA
    int temp;
    do {
        temp = areDuplicatesInVector(enemiesA);
        if (temp != -1) {
            enemiesA.erase(enemiesA.begin() + temp);
            enemiesB.erase(enemiesB.begin() + temp);
        }
    } while (temp != -1);

    //Z rund usuwam te, wśród których są duplikaty przeciwników teamuB
    do {
        temp = areDuplicatesInVector(enemiesB);
        if (temp != -1) {
            enemiesA.erase(enemiesA.begin() + temp);
            enemiesB.erase(enemiesB.begin() + temp);
        }
    } while (temp != -1);


    for (auto round: rounds) {
        for (int i = 0; i < solution.mSchedule[round].size(); i++) {
            solution.mSchedule[round][i].swapTeams(teamA, teamB);
        }
    }
}

void Solver::partialSwapTeamsPhased(Solution &solution) {
    std::mt19937 gen(mSeed);
    std::uniform_int_distribution<> distTeams(0, mProblem.mTeams.size() - 1);
    std::uniform_int_distribution<> distRounds(0, (mProblem.mSlots.size() / 2) - 1);

    int teamA = distTeams(gen);
    int teamB = distTeams(gen);

    int roundsNumber = distRounds(gen);
    std::vector<int> rounds;
    int tempRound;

    for (int i = 0; i < roundsNumber; i++) {
        do {
            tempRound = distRounds(gen);
        } while (std::find(rounds.begin(), rounds.end(), tempRound) != rounds.end());
        rounds.push_back(tempRound);
    }

    for (auto round: rounds) {
        for (int i = 0; i < solution.mSchedule[round].size(); i++) {
            solution.mSchedule[round][i].swapTeams(teamA, teamB);
        }
    }

    if (mProblem.mIsPhased) {
        if (!mProblem.mConstraints[1]->isViolated(solution)) {
            fixPhase(solution);
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
    int number_of_neighbours = 15;
    int no_definitions = 3;

    initiateRandomSolution();

    Solution bestGlobalSolution;
    bestGlobalSolution.setMSchedule(mSolutions[0].mSchedule);
    bestGlobalSolution.setMFitness(mSolutions[0].mFitness);

    Solution currSolution;
    currSolution.setMSchedule(mSolutions[0].mSchedule);
    currSolution.setMFitness(mSolutions[0].mFitness);

    Solution bestNew;


    auto temperature = mProblem.mParams.tStart;
    while (temperature > mProblem.mParams.tMin) {

        std::vector<Solution> neighboursVector;

        int innerLoopCounter = 0;
        while (innerLoopCounter < mProblem.mParams.innerLoop) {
//            std::cout<<currSolution.mFitness<<std::endl;

            //Tworzę wektor z wieloma kopiami aktualnego rozwiązania
            for (int i = 0; i < number_of_neighbours; i++) {
                Solution temp;
                temp.setMSchedule(currSolution.mSchedule);
                neighboursVector.push_back(temp);
            }

            //Generuję sąsiedztwa
            for (int i = 0; i < number_of_neighbours / no_definitions; i++) {
                partialSwapRounds(neighboursVector[i]);
                swapTeams(neighboursVector[i + no_definitions]);
                swapHomes(neighboursVector[i + no_definitions * 2]);
            }

            //Ewaluuję każde sąsiedztwo
            for (int i = 0; i < neighboursVector.size(); i++) {
                evaluate(neighboursVector[i]);
            }

            //Wybieram najlepsze z nowych rozwiązań.
            bestNew.setMSchedule(neighboursVector[0].mSchedule);
            bestNew.setMFitness(neighboursVector[0].mFitness);

            for (int i = 0; i < neighboursVector.size(); i++) {
                if (neighboursVector[i].mFitness < bestNew.mFitness) {
                    bestNew.setMSchedule(neighboursVector[i].mSchedule);
                    bestNew.setMFitness(neighboursVector[i].mFitness);
                }
            }

            //Jeśli nowe najlepsze, jest lepsze niż aktualne, akceptuję je jako currSolution
            if (bestNew.mFitness < currSolution.mFitness) {
                currSolution.setMSchedule(bestNew.mSchedule);
                currSolution.setMFitness(bestNew.mFitness);

                //Przy okazji sprawdzam, czy jest najlepszym globalnym rozwiązaniem
                if (currSolution.mFitness < bestGlobalSolution.mFitness) {
                    bestGlobalSolution.setMSchedule(currSolution.mSchedule);
                    bestGlobalSolution.setMFitness(currSolution.mFitness);
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
                }
            }
            innerLoopCounter++;
            //Zwalniam pamięć
            neighboursVector.clear();

        }
        temperature = temperature * mProblem.mParams.coolingRate;
    }
    mSolutions[0].setMSchedule(bestGlobalSolution.mSchedule);
    mSolutions[0].setMFitness(bestGlobalSolution.mFitness);
}

void Solver::fixPhase(Solution &solution) {

    //Zrobić array first matches i zamieniać spotkania first i second

    std::vector<std::vector<int>> gameVector(mProblem.mTeams.size(), std::vector<int>(mProblem.mTeams.size(), 0));

    for (int i = 0; i < mProblem.mTeams.size(); i++) {
        gameVector[i][i] = 1;
    }

    for (int round = 0; round < mProblem.mSlots.size() / 2; round++) {
        for (auto meeting: solution.mSchedule[round]) {
            gameVector[meeting.home][meeting.away] += 1;
            gameVector[meeting.away][meeting.home] += 1;
        }
    }

    for (int i = 0; i < mProblem.mTeams.size(); i++) {
        gameVector[i][i] = 0;
    }

    for (int round = mProblem.mSlots.size() / 2; round < mProblem.mSlots.size(); round++) {
        for (auto meeting: solution.mSchedule[round]) {
            gameVector[meeting.home][meeting.away] -= 1;
            gameVector[meeting.away][meeting.home] -= 1;
        }
    }
}

void Solver::evaluate(Solution &solution) {
    for (auto constraint: mProblem.mConstraints) {
        constraint->isViolated(solution);
    }
}
