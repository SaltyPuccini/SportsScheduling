#include "ConstraintBR1.h"

// Constructor
ConstraintBR1::ConstraintBR1(int intp, Mode mode1, Mode mode2, int penalty, const std::vector<int> slots,
                             int teams, ConstraintType type)
        : mIntp(intp), mMode1(mode1), mMode2(mode2), mPenalty(penalty),
          mSlots(slots), mTeams(teams), mType(type) {}

// Destructor
ConstraintBR1::~ConstraintBR1() {}

bool ConstraintBR1::isViolated(Solution) const {
    return false;
}
