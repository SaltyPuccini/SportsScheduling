#pragma once
#include "../IConstraint.h"

class ConstraintCA4 : public IConstraint {
public:
    int mMax;
    int mMin;
    Mode mMode1;
    Mode mMode2;
    int mPenalty;
    std::vector<int> mSlots;
    std::vector<int> mTeams1;
    std::vector<int> mTeams2;
    ConstraintType mType;

    // Constructor
    ConstraintCA4(int max, int min, Mode mode1, Mode mode2,
                  int penalty, const std::vector<int> slots, const std::vector<int> teams1,
                  const std::vector<int> teams2, ConstraintType type);

    // Destructor
    ~ConstraintCA4();

    // Violation check
    bool isViolated(Solution &solution) const override;
    bool isHard() const override;
};