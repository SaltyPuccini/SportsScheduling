#include "ConstraintBR1.h"

// Constructor
ConstraintBR1::ConstraintBR1(int intp, Mode mode1, Mode mode2, int penalty, const std::vector<int> slots,
                             int teams, ConstraintType type)
        : mIntp(intp), mMode1(mode1), mMode2(mode2), mPenalty(penalty),
          mSlots(slots), mTeams(teams), mType(type) {}

// Destructor
ConstraintBR1::~ConstraintBR1() {}

bool ConstraintBR1::isViolated(Solution &solution) const {
    int breaks = 0;
    bool previousRound, currentRound;
    for (auto slot: mSlots) {
        if (slot != 0) {
            for (auto meeting: solution.mSchedule[slot - 1]) {
                if (meeting.meetingContains(mTeams)) {
                    previousRound = meeting.isHome(mTeams);
                    break;
                }
            }
            for (auto meeting: solution.mSchedule[slot]) {
                if (meeting.meetingContains(mTeams)) {
                    currentRound = meeting.isHome(mTeams);
                    break;
                }
            }
            if (currentRound == previousRound) {
                breaks++;
            }
        }
    }
    if (breaks > mIntp) {
        if (mType == SOFT) {
            solution.mFitness += (breaks - mIntp) * (mPenalty * mSoft);
        } else {
            solution.mFitness += (breaks - mIntp) * (mPenalty * mHard);
        }
        return true;
    }
    return false;
}

bool ConstraintBR1::isHard() const {
    if (mType == HARD)
        return true;
    return false;
}

