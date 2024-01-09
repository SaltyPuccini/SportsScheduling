#include "PhasedConstraint.h"

PhasedConstraint::PhasedConstraint(const std::vector<int> teams, const std::vector<int> slots) : mTeams(teams),
                                                                                                 mSlots(slots) {}
PhasedConstraint::~PhasedConstraint() {}

bool PhasedConstraint::isViolated(Solution &solution) const {

    std::vector<std::vector<int>> gameVector(mTeams.size(), std::vector<int>(mTeams.size(), 0));

    for (int i = 0; i < mTeams.size(); i++){
        gameVector[i][i] = 1;
    }

    for (int round=0; round<mSlots.size()/2; round++){
        for (auto meeting : solution.mSchedule[round]){
            gameVector[meeting.home][meeting.away] += 1;
            gameVector[meeting.away][meeting.home] += 1;
        }
    }
    for (auto gameDay: gameVector){
        for (auto game : gameDay){
            if (game != 1){
                solution.mFitness += 10000;
                return true;
            }
        }
    }

    for (int i = 0; i < mTeams.size(); i++){
        gameVector[i][i] = 0;
    }

    for (int round=mSlots.size()/2; round<mSlots.size(); round++){
        for (auto meeting : solution.mSchedule[round]){
            gameVector[meeting.home][meeting.away] -= 1;
            gameVector[meeting.away][meeting.home] -= 1;
        }
    }
    for (auto gameDay: gameVector){
        for (auto game : gameDay){
            if (game != 0){
                solution.mFitness += 10000;
                return true;
            }
        }
    }


    return false;
}