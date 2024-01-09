#include "ConstraintCA1.h"

ConstraintCA1::ConstraintCA1(int max, int min, Mode mode, int penalty, const std::vector<int> slots, int teams,
                             ConstraintType type)
        : mMax(max), mMin(min), mMode(mode), mPenalty(penalty),
          mSlots(slots), mTeams(teams), mType(type) {}


ConstraintCA1::~ConstraintCA1() {
}

bool ConstraintCA1::isViolated(Solution &solution) const {
    int gamesPlayed = 0;
    for (auto slot: mSlots) {
        switch (mMode) {
            case H:
                for (int i = 0; i < solution.mSchedule[slot].size(); i++) {
                    if (solution.mSchedule[slot][i].home == mTeams) {
                        gamesPlayed++;
                    }
                }
                break;
            case A:
                for (int i = 0; i < solution.mSchedule[slot].size(); i++) {
                    if (solution.mSchedule[slot][i].away == mTeams) {
                        gamesPlayed++;
                    }
                }
                break;
            default:
                break;
        }
    }

    if (gamesPlayed > mMax) {
        if (mType == SOFT) {
            solution.mFitness += (gamesPlayed - mMax) * (mPenalty * mSoft);
        } else {
            solution.mFitness += (gamesPlayed - mMax) * (mPenalty * mHard);
            solution.setMHardViolation(true);
        }
        return true;
    }

    return false;
}

