#include "ConstraintCA4.h"

ConstraintCA4::ConstraintCA4(int max, int min, Mode mode1, Mode mode2,
                             int penalty, const std::vector<int> slots, const std::vector<int> teams1,
                             const std::vector<int> teams2, ConstraintType type)
        : mMax(max), mMin(min), mMode1(mode1), mMode2(mode2), mPenalty(penalty),
          mSlots(slots), mTeams1(teams1), mTeams2(teams2), mType(type) {}

ConstraintCA4::~ConstraintCA4() {
}

bool ConstraintCA4::isViolated(Solution &solution) const {
    std::vector<int> gameVector(solution.mSchedule[0].size() * 2, 0);
    bool isViolated = false;

    for (auto slot: mSlots) {
        switch (mMode1) {
            case H:
                for (int i = 0; i < solution.mSchedule[slot].size(); i++) {
                    for (auto homeTeam: mTeams1) {
                        for (auto enemyTeam: mTeams2) {
                            if (solution.mSchedule[slot][i].isTrulyEqual({homeTeam, enemyTeam})) {
                                gameVector[homeTeam] += 1;
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
                                gameVector[enemyTeam] += 1;
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
                                gameVector[homeTeam] += 1;
                            }
                        }
                    }
                }
                break;
            default:
                break;
        }
        if (mMode2 == EVERY) {
            int countViolated = 0;
            int resultingPenalty = 0;
            for (auto game: gameVector) {
                countViolated += game;
            }
            if (countViolated > mMax){
                isViolated = true;
                if (mType == SOFT) {
                    resultingPenalty += (countViolated - mMax) * (mPenalty * mSoft);
                } else {
                    resultingPenalty += (countViolated - mMax) * (mPenalty * mHard);
                }
            }
            solution.mFitness += resultingPenalty;
            std::fill(gameVector.begin(), gameVector.end(), 0);
        }
    }


    int resultingPenalty = 0;
    if (mMode2 == GLOBAL){
        for (auto game: gameVector) {
            if (game > mMax) {
                if (mType == SOFT) {
                    resultingPenalty += (game - mMax) * (mPenalty * mSoft);
                } else {
                    resultingPenalty += (game - mMax) * (mPenalty * mHard);
                    solution.setMHardViolation(true);
                }
            }
        }
        solution.mFitness += resultingPenalty;
    }

    if (resultingPenalty > 0 || isViolated) {
        return true;
    }
    return false;
}