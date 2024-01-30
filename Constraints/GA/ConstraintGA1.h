#pragma once
#include "../IConstraint.h"

class ConstraintGA1 : public IConstraint {
public:
    int mMax;
    tMeetings mMeetings;
    int mMin;
    int mPenalty;
    std::vector<int> mSlots;
    ConstraintType mType;

    ConstraintGA1(int max, tMeetings meetings, int min, int penalty,
                  const std::vector<int> slots, ConstraintType type);

    ~ConstraintGA1();

    bool isViolated(Solution &solution) const override;
    bool isHard() const override;
};