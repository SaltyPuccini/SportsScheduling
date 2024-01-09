#include "ConstraintBR2.h"

// Constructor
ConstraintBR2::ConstraintBR2(int intp, Mode &homeMode, Mode &mode2, int penalty, const std::vector<int> slots,
                             const std::vector<int> teams, ConstraintType type)
        : mIntp(intp), mHomeMode(homeMode), mMode2(mode2), mPenalty(penalty),
          mSlots(slots), mTeams(teams), mType(type) {}

// Destructor
ConstraintBR2::~ConstraintBR2() {
}

bool ConstraintBR2::isViolated(Solution &solution) const {

    int breaks = 0;
    bool previousRound, currentRound;
    for (auto team: mTeams) {
        for (auto slot: mSlots) {
            if (slot != 0) {
                for (auto meeting: solution.mSchedule[slot - 1]) {
                    if (meeting.meetingContains(team)) {
                        previousRound = meeting.isHome(team);
                        break;
                    }
                }
                for (auto meeting: solution.mSchedule[slot]) {
                    if (meeting.meetingContains(team)) {
                        currentRound = meeting.isHome(team);
                        break;
                    }
                }
                if (currentRound == previousRound) {
                    breaks++;
                }
            }
        }
    }

    if (breaks > mIntp) {
        if (mType == SOFT) {
            solution.mFitness += (breaks - mIntp) * (mPenalty * mSoft);
        } else {
            solution.mFitness += (breaks - mIntp) * (mPenalty * mHard);
            solution.setMHardViolation(true);
        }
        return true;
    }
    return false;
}
