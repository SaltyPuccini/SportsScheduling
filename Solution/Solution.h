#pragma once

#include "Meeting.h"

class Solution {
public:

    ~Solution(){
        mSchedule.clear();

    }
    void setMSchedule(const tSchedule &schedule);
    void setMFitness(const int fitness);


    tSchedule mSchedule;
    float mFitness = 0;

    void copyMSchedule(const tSchedule schedule);

};

