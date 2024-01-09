#include "ConstraintGA1.h"

#include <iostream>

// Constructor
ConstraintGA1::ConstraintGA1(int max, tMeetings meetings, int min, int penalty, const std::vector<int> slots,
                             ConstraintType type)
        : mMax(max), mMeetings(meetings), mMin(min), mPenalty(penalty), mSlots(slots), mType(type) {}

// Destructor
ConstraintGA1::~ConstraintGA1() {
}

bool ConstraintGA1::isViolated(Solution &solution) const {
    int gamesPlayed = 0;

    //for each slot defined in the constraint
    for (auto slotID: mSlots) {
        //for each meeting that happens during the checked slot
        for (auto meeting: solution.mSchedule[slotID]) {
            //we check if any meeting defined in the constraints happened in the defined time slot
            for (auto requiredMeeting: mMeetings) {
                if (meeting.isTrulyEqual(requiredMeeting)) {
                    gamesPlayed++;
                }
            }
        }
    }

    //if the gamesPlayed are in set range, the constraint is not violated
    if (gamesPlayed <= mMax && gamesPlayed >= mMin) {
        return false;
    }

    int multiplier = mSoft;
    if (mType == HARD) {
        multiplier = mHard;
        solution.setMHardViolation(true);
    }

    //if not, add penalty to the fitness of the solution and return info that the constraint indeed is violated
    if (gamesPlayed < mMin) {
        solution.mFitness += (mMin - gamesPlayed) * (mPenalty * multiplier);
    } else if (gamesPlayed > mMax) {
        solution.mFitness += (gamesPlayed - mMax) * (mPenalty * multiplier);
    }
    return true;
}

