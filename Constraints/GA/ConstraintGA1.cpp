#include "ConstraintGA1.h"

#include <iostream>

// Constructor
ConstraintGA1::ConstraintGA1(int max, tMeetings meetings, int min, int penalty, const std::vector<int> slots,
                             ConstraintType type)
        : mMax(max), mMeetings(meetings), mMin(min), mPenalty(penalty), mSlots(slots), mType(type) {}

// Destructor
ConstraintGA1::~ConstraintGA1() {
}

bool ConstraintGA1::isViolated(Solution) const {
    return false;
}
