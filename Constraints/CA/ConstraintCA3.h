#pragma once
#include "../IConstraint.h"

class ConstraintCA3 : public IConstraint {
public:
    int mIntp;
    int mMax;
    int mMin;
    Mode mMode1;
    Mode mMode2;
    int mPenalty;
    std::vector<int> mTeams1;
    std::vector<int> mTeams2;
    ConstraintType mType;

    ConstraintCA3(int intp, int max, int min, Mode mode1, Mode mode2,
                  int penalty, const std::vector<int> teams1, const std::vector<int> teams2,
                  ConstraintType type);

    ~ConstraintCA3();

    bool isViolated(Solution &solution) const override;
    bool isHard() const override;
};
