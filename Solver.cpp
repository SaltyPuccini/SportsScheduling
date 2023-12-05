#include <unordered_set>
#include <algorithm>
#include <random>
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
    mSolutions.push_back(solution);
}

void Solver::setSeed(int seed) {
    mSeed = seed;
}

void Solver::swapRounds(Solution &solution) {
    std::mt19937 gen(mSeed);
    std::uniform_int_distribution<> dist(0, mProblem.mSlots.size() - 1);
    int roundA = dist(gen);
    int roundB = dist(gen);

    auto temp = solution.mSchedule[roundA];
    solution.mSchedule[roundA] = solution.mSchedule[roundB];
    solution.mSchedule[roundB] = temp;
}

void Solver::swapTeams(Solution &solution) {
    std::mt19937 gen(mSeed);
    std::uniform_int_distribution<> dist(0, mProblem.mTeams.size() - 1);
    int teamA = dist(gen);
    int teamB = dist(gen);

    for (auto round: solution.mSchedule) {
        for (auto meeting: round)
            meeting.swapTeams(teamA, teamB);
    }
}

void Solver::swapHomes(Solution &solution) {
    std::mt19937 gen(mSeed);
    std::uniform_int_distribution<> dist(0, mProblem.mTeams.size() - 1);
    int teamA = dist(gen);
    int teamB = dist(gen);

    for (auto round: solution.mSchedule) {
        for (auto meeting: round)
            if (meeting.isPartiallyEqual({teamA, teamB})) {
                meeting.swapHomeAway();
            }
    }
}

void Solver::anneal() {

}
