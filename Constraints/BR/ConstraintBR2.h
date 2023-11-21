#pragma once
#include "../IConstraint.h"

class ConstraintBR2 : public IConstraint {
public:
    int mIntp;
    Mode mHomeMode;
    Mode mMode2;
    int mPenalty;
    std::vector<int> mSlots;
    std::vector<int> mTeams;
    ConstraintType mType;

    // Constructor
    ConstraintBR2(int intp, Mode &homeMode, Mode &mode2, int penalty,
                  const std::vector<int> slots, const std::vector<int> teams,
                  ConstraintType type);

    // Destructor
    ~ConstraintBR2();

    bool isViolated(Solution) const override;

};
