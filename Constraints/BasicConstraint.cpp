#include "BasicConstraint.h"

BasicConstraint::BasicConstraint(const std::vector<int> teams, const std::vector<int> slots,
                                 ConstraintType constraintType) : mTeams(teams),
                                                                  mSlots(slots), mType(constraintType) {}

BasicConstraint::~BasicConstraint() {}

bool BasicConstraint::isViolated(Solution &solution) const {
    std::vector<int> gameVector(mTeams.size(), 0);

    for (auto round: solution.mSchedule) {
        for (auto meeting: round) {
            gameVector[meeting.home] += 1;
            gameVector[meeting.away] += 1;
        }

        for (auto isPlayed: gameVector) {
            if (isPlayed != 1) {
                return true;
            }
        }
        std::fill(gameVector.begin(), gameVector.end(), 0);
    }


    return false;
}

bool BasicConstraint::isHard() const {
    if (mType == HARD)
        return true;
    return false;
}