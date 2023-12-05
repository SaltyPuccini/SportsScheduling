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

    bool isHome(const int &team) {
        if (home == team) {
            return true;
        } else {
            return false;
        }
    }

    void swapHomeAway() {
        auto temp = home;
        home = away;
        away = temp;
    }

    void swapTeams(int teamA, int teamB) {
        if (meetingContains(teamA)){
            if(isHome(teamA)){
                home = teamB;
            }else{
                away = teamB;
            }
        }
        if (meetingContains(teamB)){
            if(isHome(teamB)){
                home = teamA;
            }else{
                away = teamA;
            }
        }
    }
};

typedef std::vector<Meeting> tMeetings;
typedef std::vector<tMeetings> tSchedule;