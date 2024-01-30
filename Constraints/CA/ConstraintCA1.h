#pragma once

#include "../IConstraint.h"

class ConstraintCA1 : public IConstraint {
public:
    int mMax;
    int mMin;
    Mode mMode;
    int mPenalty;
    std::vector<int> mSlots;
    int mTeams;
    ConstraintType mType;

    ConstraintCA1(int max, int min, Mode mode, int penalty,
                  const std::vector<int> slots, int teams, ConstraintType type);

    ~ConstraintCA1();

    bool isViolated(Solution &solution) const override;
    bool isHard() const override;
};
