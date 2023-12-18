#include "PhasedConstraint.h"

PhasedConstraint::PhasedConstraint(const std::vector<int> teams, const std::vector<int> slots) : mTeams(teams),
                                                                                                 mSlots(slots) {}
PhasedConstraint::~PhasedConstraint() {}

bool PhasedConstraint::isViolated(Solution &solution) const {
    int numTeams = solution.mSchedule[0].size() * 2;
    std::vector<int> gameVector(numTeams, 0);

    for (auto round : solution.mSchedule){
        for (auto meeting : round){
            gameVector[meeting.home] += 1;
            gameVector[meeting.away] -= 1;
        }
    }


    return false;
}