#pragma once
#include <vector>

struct Meeting {
    int home;
    int away;

    bool isTrulyEqual(const Meeting& other) const {
        return (home == other.home) && (away == other.away);
    }

    bool isPartiallyEqual(const Meeting& other) const {
        return (((home == other.home) && (away == other.away)) || ((home == other.away) && (away == other.home)));
    }

};
typedef std::vector<Meeting> tMeetings;
typedef std::vector<tMeetings> tSchedule;