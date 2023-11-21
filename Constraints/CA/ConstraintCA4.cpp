#include "ConstraintCA4.h"

ConstraintCA4::ConstraintCA4(int max, int min, Mode mode1, Mode mode2,
                             int penalty, const std::vector<int> slots, const std::vector<int> teams1,
                             const std::vector<int> teams2, ConstraintType type)
        : mMax(max), mMin(min), mMode1(mode1), mMode2(mode2), mPenalty(penalty),
          mSlots(slots), mTeams1(teams1), mTeams2(teams2), mType(type) {}

ConstraintCA4::~ConstraintCA4() {
}

bool ConstraintCA4::isViolated(Solution) const {
    return false;
}
