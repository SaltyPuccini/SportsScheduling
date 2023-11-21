#include "ConstraintFA2.h"
#include <iostream>

// Constructor
ConstraintFA2::ConstraintFA2(int intp, Mode mode, int penalty, const std::vector<int> slots,
                             const std::vector<int> teams, ConstraintType type)
        : mIntp(intp), mMode(mode), mPenalty(penalty),
          mSlots(slots), mTeams(teams), mType(type) {}

// Destructor
ConstraintFA2::~ConstraintFA2() {

}

bool ConstraintFA2::isViolated(Solution) const {
    return false;
}
