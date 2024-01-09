#include <unordered_set>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <set>
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

bool areSchedulesEqual(tSchedule &schedule1, tSchedule &schedule2, int ra, int rb) {
    for (int j = 0; j < schedule1[ra].size(); ++j) {
        if (!schedule1[ra][j].isTrulyEqual(schedule2[ra][j])) {
            return false;
        }
        if (!schedule1[rb][j].isTrulyEqual(schedule2[rb][j])) {
            return false;
        }
    }

    return true;
}


void Solver::teamsPlayOnlyInTs(Solution &solution, int ra, int rb,
                               std::unordered_set<int> &Ts) {
    bool teamsRemoved = true;

    while (teamsRemoved) {
        teamsRemoved = false;

        for (auto it = Ts.begin(); it != Ts.end();) {
            int team = *it;
            bool playsOnlyInTs = true;

            // Sprawdzamy czy drużyna gra tylko z drużynami z Ts w rundzie ra
            for (auto meeting: solution.mSchedule[ra]) {
                if (meeting.meetingContains(team)) {
                    if (Ts.find(meeting.whoIsEnemy(team)) == Ts.end()) {
                        playsOnlyInTs = false;
                        break;
                    }
                }
            }

            // Sprawdzamy czy drużyna gra tylko z drużynami z Ts w rundzie rb
            for (auto meeting: solution.mSchedule[rb]) {
                if (meeting.meetingContains(team)) {
                    if (Ts.find(meeting.whoIsEnemy(team)) == Ts.end()) {
                        playsOnlyInTs = false;
                        break;
                    }
                }
            }

            if (!playsOnlyInTs) {
                it = Ts.erase(it); // Usuń drużynę z Ts
                teamsRemoved = true;
            } else {
                ++it;
            }
        }
    }
}


bool pairCompare(const std::pair<int, int> &lhs, const std::pair<int, int> &rhs) {
    return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
}


void Solver::partialSwapRounds(Solution &solution) {
    std::uniform_int_distribution<> distRounds(0, mProblem.mSlots.size() - 1);
    std::uniform_int_distribution<> distTeamsCount(2, mProblem.mTeams.size() - 2);
    std::uniform_int_distribution<> distTeams(0, (mProblem.mTeams.size() - 1));

    Solution tempSolution;
    tempSolution.copyMSchedule(solution.mSchedule);

    //Losowanie rund
    int roundA, roundB;
    do {
        roundA = distRounds(gen);
        roundB = distRounds(gen);
    } while (roundA == roundB);


    int teamsNumber = distTeamsCount(gen);
    std::unordered_set<int> Ts;
    int tempTeam;

    //losowanie teamów
    for (int i = 0; i < teamsNumber; i++) {
        do {
            tempTeam = distTeams(gen);
        } while (std::find(Ts.begin(), Ts.end(), tempTeam) != Ts.end());
        Ts.insert(tempTeam);
    }

    //modyfikujemy tak, że usuwamy teamy grające chociaż raz z kimkolwiek z poza TS
    teamsPlayOnlyInTs(solution, roundA, roundB, Ts);
    std::unordered_map<int, std::vector<int>> matchesRa, matchesRb;

    //mapa meczy roundA
    for (int matchIndex = 0; matchIndex < solution.mSchedule[roundA].size(); ++matchIndex) {
        int team = solution.mSchedule[roundA][matchIndex].home;
        if (Ts.find(team) != Ts.end()) {
            matchesRa[team].push_back(matchIndex);
        }

        team = solution.mSchedule[roundA][matchIndex].away;
        if (Ts.find(team) != Ts.end()) {
            matchesRa[team].push_back(matchIndex);
        }
    }

    //mapa meczy roundB
    for (int matchIndex = 0; matchIndex < solution.mSchedule[roundB].size(); ++matchIndex) {
        int team = solution.mSchedule[roundB][matchIndex].home;
        if (Ts.find(team) != Ts.end()) {
            matchesRb[team].push_back(matchIndex);
        }

        team = solution.mSchedule[roundB][matchIndex].away;
        if (Ts.find(team) != Ts.end()) {
            matchesRb[team].push_back(matchIndex);
        }
    }


    std::set<std::pair<int, int>, decltype(&pairCompare)> swappedMatches(&pairCompare);

    // Zamiana meczów pomiędzy ri i rj dla drużyn z Ts (tylko raz)
    for (int team: Ts) {
        if (matchesRa.find(team) != matchesRa.end() && matchesRb.find(team) != matchesRb.end()) {
            // Swap only once if the pair hasn't been swapped before
            for (int matchIndexRa: matchesRa[team]) {
                int matchIndexRb = matchesRb[team][0]; // Take the first match in rj for simplicity

                std::pair<int, int> forwardSwap = {matchIndexRa, matchIndexRb};
                std::pair<int, int> backwardSwap = {matchIndexRb, matchIndexRa};
                if (swappedMatches.find(forwardSwap) == swappedMatches.end() &&
                    swappedMatches.find(backwardSwap) == swappedMatches.end()) {
                    std::swap(solution.mSchedule[roundA][matchIndexRa], solution.mSchedule[roundB][matchIndexRb]);
                    swappedMatches.insert(forwardSwap);
                }
            }
        }
    }

//    if (!Ts.empty()){
//        auto temp = mProblem.mConstraints[0]->isViolated(solution);
//        if (!areSchedulesEqual(solution.mSchedule, tempSolution.mSchedule, roundA, roundB)){
//            std::cout << "Are equal: " << areSchedulesEqual(solution.mSchedule, tempSolution.mSchedule, roundA, roundB) << std::endl;
//            std::cout << "Break rules: " << temp << std::endl;
//        }
//    }
}


bool Solver::CanSwapTeams(const Solution &solution,
                          int ti, int tj, const std::vector<int> &Rs) {
    std::unordered_map<int, std::unordered_map<int, int>> opponentMatchCount;

    // Zliczanie meczów przeciwko wszystkim przeciwnikom dla ti i tj
    for (int round: Rs) {
        for (const auto &match: solution.mSchedule[round]) {
            int team1 = match.home;
            int team2 = match.away;

            if (team1 == ti || team1 == tj) {
                opponentMatchCount[team2][team1]++;
            }
            if (team2 == ti || team2 == tj) {
                opponentMatchCount[team1][team2]++;
            }
        }
    }

    for (const auto &teamMatches: opponentMatchCount) {
        for (const auto &count: teamMatches.second) {
            if (count.first == ti || count.first == tj)
                continue;
            if (opponentMatchCount[teamMatches.first][ti] != opponentMatchCount[teamMatches.first][tj]) {
                return false;
            }
        }
    }

    return true;
}


void Solver::partialSwapTeams(Solution &solution) {

    std::uniform_int_distribution<> distRounds(0, mProblem.mSlots.size() - 1);
//    std::uniform_int_distribution<> distRoundsCount(2, mProblem.mSlots.size() - 2);
    std::uniform_int_distribution<> distRoundsCount(2, 7);
    std::uniform_int_distribution<> distTeams(0, (mProblem.mTeams.size() - 1));

    //Losowanie teamów
    int ti, tj;
    do {
        ti = distTeams(gen);
        tj = distTeams(gen);
    } while (ti == tj);

    std::vector<int> Rs;

    auto roundCount = distRoundsCount(gen);
    int tempRound;
    //losowanie rund
    for (int i = 0; i < roundCount; i++) {
        do {
            tempRound = distRounds(gen);
        } while (std::find(Rs.begin(), Rs.end(), tempRound) != Rs.end());
        Rs.push_back(tempRound);
    }

    if (CanSwapTeams(solution, ti, tj, Rs)) {
        for (int round: Rs) {
            for (auto &match: solution.mSchedule[round]) {
                match.swapTeams(ti, tj);
            }
        }
    }

    auto temp = mProblem.mConstraints[0]->isViolated(solution);
    std::cout << temp << std::endl;
}


void Solver::partialSwapTeamsPhased(Solution &solution) {
    partialSwapTeams(solution);
    fixPhase(solution);
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
//    int number_of_definitions = 1;
    int number_of_definitions = 5;
    int number_of_neighbours = 5 * number_of_definitions;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_neigh(0, number_of_neighbours - 1);

    initiateRandomSolution();

    Solution bestGlobalSolution;
    bestGlobalSolution.setMSchedule(mSolutions[0].mSchedule);
    bestGlobalSolution.setMFitness(mSolutions[0].mFitness);

    Solution currSolution;
    currSolution.setMSchedule(mSolutions[0].mSchedule);
    currSolution.setMFitness(mSolutions[0].mFitness);

    Solution bestNew;
    unsigned int allCounter = 0;


    auto temperature = mProblem.mParams.tStart;
    while (temperature > mProblem.mParams.tMin) {

        std::vector<Solution> neighboursVector;

        int innerLoopCounter = 0;
        while (innerLoopCounter < mProblem.mParams.innerLoop) {
            //Tworzę wektor z wieloma kopiami aktualnego rozwiązania
            for (int i = 0; i < number_of_neighbours; i++) {
                Solution temp;
                temp.setMSchedule(currSolution.mSchedule);
                neighboursVector.push_back(temp);
            }

            //Generuję sąsiedztwa
            for (int i = 0; i < number_of_neighbours / number_of_definitions; i++) {
                swapTeams(neighboursVector[i]);
                swapHomes(neighboursVector[i + number_of_definitions]);
                swapRounds(neighboursVector[i + number_of_definitions * 2]);
                partialSwapRounds(neighboursVector[i + number_of_definitions * 3]);
                partialSwapTeams(neighboursVector[i + number_of_definitions * 4]);
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


            for (int i = 0; i < neighboursVector.size(); i++) {
                if (neighboursVector[i].mFitness < bestNew.mFitness) {
                    bestNew.setMSchedule(neighboursVector[i].mSchedule);
                    bestNew.setMFitness(neighboursVector[i].mFitness);
                    bestID = i;
                }
            }

//            if (bestID<5)
//                std::cout<<"SwapTeams"<<std::endl;
//            else if (bestID<10)
//                std::cout<<"swapHomes"<<std::endl;
//            else if (bestID<15)
//                std::cout<<"swapRounds"<<std::endl;
//            else if (bestID<20)
//                std::cout<<"partialSwapRounds"<<std::endl;

            //Jeśli nowe najlepsze, jest lepsze niż aktualne, akceptuję je jako currSolution
            if (bestNew.mFitness < currSolution.mFitness) {
                currSolution.setMSchedule(bestNew.mSchedule);
                currSolution.setMFitness(bestNew.mFitness);

                //Przy okazji sprawdzam, czy jest najlepszym globalnym rozwiązaniem
                if (currSolution.mFitness < bestGlobalSolution.mFitness) {
                    bestGlobalSolution.setMSchedule(currSolution.mSchedule);
                    bestGlobalSolution.setMFitness(currSolution.mFitness);
                    std::cout << "Mamy nowe" << std::endl;
                }
            }
//            else if (bestNew.mFitness == currSolution.mFitness){
//                currSolution.setMSchedule(neighboursVector[dis_neigh(gen)].mSchedule);
//                currSolution.setMFitness(neighboursVector[dis_neigh(gen)].mFitness);
//            }
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
            allCounter++;
            innerLoopCounter++;
            std::cout << currSolution.mFitness << std::endl;
            std::cout << bestGlobalSolution.mFitness << std::endl;

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
