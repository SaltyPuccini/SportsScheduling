#pragma once

#include <vector>

struct Meeting {
    int home;
    int away;

    bool isTrulyEqual(const Meeting &other) const {
        return (home == other.home) && (away == other.away);
    }

    bool isPartiallyEqual(const Meeting &other) const {
        return (((home == other.home) && (away == other.away)) || ((home == other.away) && (away == other.home)));
    }

    bool meetingContains(const int &team) const {
        return ((home == team) || (away == team));
    }

    Mode homeOrAway(const int &team) {
        if (home == team) {
            return H;
        } else if (away == team){
            return A;
        }
        return NONE;
    }

};

typedef std::vector<Meeting> tMeetings;
typedef std::vector<tMeetings> tSchedule;