#pragma once

#include "Meeting.h"

class Solution {
public:
    void setMSchedule(const tSchedule &schedule);

    tSchedule mSchedule;
    float mFitness = 0;
/*
    schedule[n] to inaczej slot = n

    round 0     1;2, 3;4, 5;6
    round 1     1;3, 2;5, 4;6
    round 2     1:4, 2;6, 3;5
    round 3     1:5, 2;4, 3;6
    round 4     1:6, 2;3, 4;5
    round 5     2:1, 4;2, 6;5
    round 6     3:1, 5;2, 6;4
    round 7     4:1, 6;2, 5;3
    round 8     5:1, 4;2, 6;3
    round 9     6:1, 3;2, 5;4
 */
};

