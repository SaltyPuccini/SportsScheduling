#include "ConstraintCA2.h"

ConstraintCA2::ConstraintCA2(int max, int min, Mode mode1, Mode mode2,
                             int penalty, const std::vector<int> slots, int teams1,
                             const std::vector<int> teams2, ConstraintType type)
        : mMax(max), mMin(min), mMode1(mode1), mMode2(mode2), mPenalty(penalty),
          mSlots(slots), mTeams1(teams1), mTeams2(teams2), mType(type) {}

ConstraintCA2::~ConstraintCA2() {
}

bool ConstraintCA2::isViolated(Solution &solution) const {
    int gamesPlayed = 0;
    for (auto slot: mSlots) {
        switch (mMode1) {
            case H:
                for (int i = 0; i < solution.mSchedule[slot].size(); i++) {
                    for (auto enemyTeam: mTeams2) {
                        if (solution.mSchedule[slot][i].isTrulyEqual({mTeams1, enemyTeam})) {
                            gamesPlayed++;
                        }
                    }
                }
                break;
            case A:
                for (int i = 0; i < solution.mSchedule[slot].size(); i++) {
                    for (auto enemyTeam: mTeams2) {
                        if (solution.mSchedule[slot][i].isTrulyEqual({enemyTeam, mTeams1})) {
                            gamesPlayed++;
                        }
                    }
                }
                break;
            case HA:
                for (int i = 0; i < solution.mSchedule[slot].size(); i++) {
                    for (auto enemyTeam: mTeams2) {
                        if (solution.mSchedule[slot][i].isPartiallyEqual({mTeams1, enemyTeam})) {
                            gamesPlayed++;
                        }
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

