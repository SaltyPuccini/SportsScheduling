#include "Solution.h"

void Solution::setMSchedule(const tSchedule &schedule) {
    Solution::mSchedule = schedule;
}

void Solution::copyMSchedule(const tSchedule schedule) {
    Solution::mSchedule = schedule;
}

void Solution::setMFitness(const int fitness) {
    Solution::mFitness = fitness;
}

void Solution::setMHardViolation(bool violated) {
    Solution::mHardViolation = violated;
}

