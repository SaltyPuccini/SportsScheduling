#pragma once
#include <vector>

struct Meeting {
    int home;
    int away;
};
typedef std::vector<Meeting> tMeetings;
typedef std::vector<tMeetings> tSchedule;