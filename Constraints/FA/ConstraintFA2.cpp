#include "ConstraintFA2.h"
#include <iostream>

// Constructor
ConstraintFA2::ConstraintFA2(int intp, Mode mode, int penalty, const std::vector<int> slots,
                             const std::vector<int> teams, ConstraintType type)
        : mIntp(intp), mMode(mode), mPenalty(penalty),
          mSlots(slots), mTeams(teams), mType(type) {}

// Destructor
ConstraintFA2::~ConstraintFA2() {

}

bool ConstraintFA2::isViolated(Solution &solution) const {
    int penalty = 0;
    //Vector which counts amount of home games played for each time slot.
    std::vector<std::vector<int>> homeGames(mSlots.size(), std::vector<int>(mTeams.size(), 0));

    for (auto meeting: solution.mSchedule[0]) {
        homeGames[0][meeting.home]++;
    }

    for (int i = 1; i < solution.mSchedule.size(); i++) {
        for (auto meeting: solution.mSchedule[i]) {
            homeGames[i][meeting.home] = homeGames[i - 1][meeting.home] + 1;
            homeGames[i][meeting.away] = homeGames[i - 1][meeting.away];
        }
    }

    //after populating the vector we can chceck the violation for each pair of teams after every time slot

    //Each pair of teams in teams triggers a deviation equal
    //to the largest difference in played home games more than
    //intp over all time slots in slots.

    for (int i = 0; i < mTeams.size(); i++) {
        for (int j = i + 1; j < mTeams.size(); j++) {

            int max = 0;

            for (int slotId = 0; slotId < mSlots.size(); slotId++) {
                if (std::abs(homeGames[slotId][i] - homeGames[slotId][j]) > max) {
                    max = std::abs(homeGames[slotId][i] - homeGames[slotId][j]);
                }
            }

            if (max > mIntp){
                if (mType == SOFT) {
                    penalty += max * (mPenalty * mSoft);
                } else {
                    solution.setMHardViolation(true);
                    penalty += max * (mPenalty * mHard);
                }
            }
        }
    }

    if (penalty != 0) {
        solution.mFitness += penalty;
        return true;
    }

    return false;


}
