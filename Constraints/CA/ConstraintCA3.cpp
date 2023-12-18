#include "ConstraintCA3.h"

ConstraintCA3::ConstraintCA3(int intp, int max, int min, Mode mode1, Mode mode2,
                             int penalty, const std::vector<int> teams1, const std::vector<int> teams2,
                             ConstraintType type)
        : mIntp(intp), mMax(max), mMin(min), mMode1(mode1), mMode2(mode2), mPenalty(penalty),
          mTeams1(teams1), mTeams2(teams2), mType(type) {}

ConstraintCA3::~ConstraintCA3() {
}

bool ConstraintCA3::isViolated(Solution &solution) const {
    std::vector<int> gameVector(solution.mSchedule[0].size() * 2, 0);
    std::vector<int> lastPlayed(solution.mSchedule[0].size() * 2, -1);
    for (int slot = 0; slot < solution.mSchedule.size(); slot++) {
        switch (mMode1) {
            case H:
                for (int i = 0; i < solution.mSchedule[slot].size(); i++) {
                    for (auto homeTeam: mTeams1) {
                        for (auto enemyTeam: mTeams2) {
                            if (solution.mSchedule[slot][i].isTrulyEqual({homeTeam, enemyTeam})) {
                                if (lastPlayed[homeTeam] == -1) {
                                    gameVector[homeTeam] += 1;
                                } else if (slot < lastPlayed[homeTeam] + mIntp) {
                                    gameVector[homeTeam] += 1;
                                }
                                lastPlayed[homeTeam] = slot;
                            }
                        }
                    }
                }
                break;
            case A:
                for (int i = 0; i < solution.mSchedule[slot].size(); i++) {
                    for (auto enemyTeam: mTeams1) {
                        for (auto homeTeam: mTeams2) {
                            if (solution.mSchedule[slot][i].isTrulyEqual({homeTeam, enemyTeam})) {
                                lastPlayed[enemyTeam] = slot;
                                if (slot < lastPlayed[enemyTeam] + mIntp) {
                                    gameVector[enemyTeam] += 1;
                                }
                            }
                        }
                    }
                }
                break;
            case HA:
                for (int i = 0; i < solution.mSchedule[slot].size(); i++) {
                    for (auto homeTeam: mTeams1) {
                        for (auto enemyTeam: mTeams2) {
                            if (solution.mSchedule[slot][i].isPartiallyEqual({homeTeam, enemyTeam})) {
                                lastPlayed[homeTeam] = slot;
                                if (slot < lastPlayed[homeTeam] + mIntp) {
                                    gameVector[homeTeam] += 1;
                                }
                            }
                        }
                    }
                }
                break;
            default:
                break;
        }
    }

    int resultingPenalty = 0;

    for (auto game: gameVector) {
        if (game > mMax) {
            if (mType == SOFT) {
                resultingPenalty += (game - mMax) * (mPenalty * mSoft);
            } else {
                resultingPenalty += (game - mMax) * (mPenalty * mHard);
            }
        }
    }

    if (resultingPenalty > 0) {
        return true;
    }
    return false;
}

