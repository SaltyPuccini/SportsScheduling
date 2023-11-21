#pragma once
#include "Meeting.h"

class Solution {
public:
    float getFitness() const;
    void setMSchedule(const tSchedule &schedule);

private:
/*
    schedule[n] to inaczej slot = n

    slot 0     1;2, 3;4, 5;6
    slot 1     1;3, 2;5, 4;6
    slot 2     1:4, 2;6, 3;5
    slot 3     1:5, 2;4, 3;6
    slot 4     1:6, 2;3, 4;5
    slot 5     2:1, 4;2, 6;5
    slot 6     3:1, 5;2, 6;4
    slot 7     4:1, 6;2, 5;3
    slot 8     5:1, 4;2, 6;3
    slot 9     6:1, 3;2, 5;4
 */
private:
    float mFitness;
    tSchedule mSchedule;
};

