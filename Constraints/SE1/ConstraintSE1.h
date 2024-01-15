#pragma once
#include "../IConstraint.h"

class ConstraintSE1 : public IConstraint {
public:
    ConstraintSE1(const Mode mode1, int min, int penalty, const std::vector<int> teams, const ConstraintType type);

    Mode mMode1;
    int mMin;
    int mPenalty;
    std::vector<int> mTeams;
    ConstraintType mType;


    // Destructor
    ~ConstraintSE1();

    // Violation check
    bool isViolated(Solution &solution) const override;
    bool isHard() const override;
};
