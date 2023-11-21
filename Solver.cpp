#include <unordered_set>
#include <algorithm>
#include <random>
#include "Solver.h"

Solver::~Solver() {
}


void Solver::initiateSolution() {
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

    mSolution.setMSchedule(schedule);

}


void Solver::randomSolution(int repetition) {

    int rounds = (mProblem.mTeams.size() - 1) * 2;
    int size = mProblem.mTeams.size();

    int meetingsPerTimeslot = mProblem.mTeams.size() / 2;
    tSchedule schedule(mProblem.mSlots.size(), tMeetings(meetingsPerTimeslot));


    std::vector<Meeting> possibleMeetings;
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            Meeting m1 = {i, j};
            Meeting m2 = {j, i};
            possibleMeetings.push_back(m1);
            possibleMeetings.push_back(m2);
        }
    }

    for (int seed = 0; seed < repetition; ++seed) {
        std::mt19937 mt(seed);
        std::shuffle(possibleMeetings.begin(), possibleMeetings.end(), mt);
        int overallLoop = 0;
        for (int round = 0; round < rounds; ++round) {
            for (int game = 0; game < meetingsPerTimeslot; ++game) {
                schedule[round][game] = possibleMeetings[overallLoop];
                overallLoop++;
            }
        }
        auto temp = schedule;
    }



}


