#include "ConstraintBR2.h"

// Constructor
ConstraintBR2::ConstraintBR2(int intp, Mode &homeMode, Mode &mode2, int penalty, const std::vector<int> slots,
                             const std::vector<int> teams, ConstraintType type)
        : mIntp(intp), mHomeMode(homeMode), mMode2(mode2), mPenalty(penalty),
          mSlots(slots), mTeams(teams), mType(type) {}

// Destructor
ConstraintBR2::~ConstraintBR2() {
}

bool ConstraintBR2::isViolated(Solution) const {
    return false;
}
