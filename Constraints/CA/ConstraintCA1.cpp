#include "ConstraintCA1.h"

ConstraintCA1::ConstraintCA1(int max, int min, Mode mode, int penalty, const std::vector<int> slots, int teams,
                             ConstraintType type)
        : mMax(max), mMin(min), mMode(mode), mPenalty(penalty),
          mSlots(slots), mTeams(teams), mType(type) {}



ConstraintCA1::~ConstraintCA1() {
}

bool ConstraintCA1::isViolated(Solution &solution) const {
    return false;
}

