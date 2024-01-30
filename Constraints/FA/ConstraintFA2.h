#pragma once
#include "../IConstraint.h"

class ConstraintFA2 : public IConstraint {
public:
    int mIntp;
    Mode mMode;
    int mPenalty;
    std::vector<int> mSlots;
    std::vector<int> mTeams;
    ConstraintType mType;

    ConstraintFA2(int intp, Mode mode, int penalty,
                  const std::vector<int> slots, const std::vector<int> teams,
                  ConstraintType type);


    ~ConstraintFA2();

    bool isViolated(Solution &solution) const override;
    bool isHard() const override;
};

