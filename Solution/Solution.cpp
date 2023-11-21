#include "Solution.h"

float Solution::getFitness() const {
    return mFitness;
}

void Solution::setMSchedule(const tSchedule &schedule) {
    Solution::mSchedule = schedule;
}

