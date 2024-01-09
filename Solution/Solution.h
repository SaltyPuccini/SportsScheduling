#pragma once

#include "Meeting.h"

class Solution {
public:

    ~Solution(){
        mSchedule.clear();

    }
    void setMSchedule(const tSchedule &schedule);
    void setMFitness(const int fitness);
    void setMHardViolation(bool violated);


    tSchedule mSchedule;
    float mFitness = 0;
    bool mHardViolation;

    void copyMSchedule(const tSchedule schedule);
};

