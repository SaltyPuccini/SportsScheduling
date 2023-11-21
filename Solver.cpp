#include <unordered_set>
#include <algorithm>
#include "Solver.h"

Solver::~Solver() {
}


void Solver::initiateSolution() {
    int meetingsPerTimeslot = mProblem.mTeams.size() / 2;
    std::vector<std::unordered_set<int>> plannedMeetings(mProblem.mSlots.size());
    tSchedule schedule(mProblem.mSlots.size(), tMeetings(meetingsPerTimeslot));

    auto teams = mProblem.mTeams;

    int numTeams = teams.size();
    int rounds = (teams.size() - 1) * 2;

    for (int round = 0; round < rounds; ++round) {
        for (int game = 0; game < meetingsPerTimeslot; ++game) {
            int team1 = teams[game];
            int team2 = teams[numTeams - 1 - game];


            if(round < teams.size() - 1){
                Meeting temp = {team1, team2};
                schedule[round][game] = temp;
            }else{
                Meeting temp = {team2, team1};
                schedule[round][game] = temp;
            }

        }
        std::rotate(teams.begin() + 1, teams.begin() + numTeams - 1, teams.end());
    }

    mSolution.setMSchedule(schedule);

}

