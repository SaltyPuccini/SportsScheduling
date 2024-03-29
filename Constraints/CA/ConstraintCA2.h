#pragma once
#include "../IConstraint.h"

class ConstraintCA2 : public IConstraint {
public:
    int mMax;
    int mMin;
    Mode mMode1;
    Mode mMode2;
    int mPenalty;
    std::vector<int> mSlots;
    int mTeams1;
    std::vector<int> mTeams2;
    ConstraintType mType;

    ConstraintCA2(int max, int min, Mode mode1, Mode mode2,
                  int penalty, const std::vector<int> slots, int teams1,
                  const std::vector<int> teams2, ConstraintType type);

    ~ConstraintCA2();

    bool isViolated(Solution &solution) const override;
    bool isHard() const override;
};