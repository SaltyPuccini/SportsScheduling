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

    // Constructor
    ConstraintCA1(int max, int min, Mode mode, int penalty,
                  const std::vector<int> slots, int teams, ConstraintType type);

    // Destructor
    ~ConstraintCA1();

    // Violation check
    bool isViolated(Solution &solution) const override;
};
