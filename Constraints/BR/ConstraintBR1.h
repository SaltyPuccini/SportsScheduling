#pragma once
#include "../IConstraint.h"

class ConstraintBR1 : public IConstraint {
public:
    int mIntp;
    Mode mMode1;
    Mode mMode2;
    int mPenalty;
    std::vector<int> mSlots;
    int mTeams;
    ConstraintType mType;

    // Constructor
    ConstraintBR1(int intp, Mode mode1, Mode mode2, int penalty,
                  const std::vector<int> slots, int teams,
                  ConstraintType type);

    // Destructor
    ~ConstraintBR1();

    bool isViolated(Solution) const override;
};
