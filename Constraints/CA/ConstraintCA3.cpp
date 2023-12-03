#include "ConstraintCA3.h"

ConstraintCA3::ConstraintCA3(int intp, int max, int min, Mode mode1, Mode mode2,
                             int penalty, const std::vector<int> teams1, const std::vector<int> teams2,
                             ConstraintType type)
        : mIntp(intp), mMax(max), mMin(min), mMode1(mode1), mMode2(mode2), mPenalty(penalty),
          mTeams1(teams1), mTeams2(teams2), mType(type) {}

ConstraintCA3::~ConstraintCA3() {
}

bool ConstraintCA3::isViolated(Solution &solution) const {
    return false;
}

