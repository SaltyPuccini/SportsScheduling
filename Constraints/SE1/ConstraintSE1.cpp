#include "ConstraintSE1.h"

// Constructor implementation
ConstraintSE1::ConstraintSE1(const Mode mode1, int min, int penalty, const std::vector<int> teams,
                             const ConstraintType type)
        : mMode1(mode1), mMin(min), mPenalty(penalty), mTeams(teams), mType(type) {}

// Destructor implementation
ConstraintSE1::~ConstraintSE1() {
}

bool ConstraintSE1::isViolated(Solution) const {
    return false;
}
