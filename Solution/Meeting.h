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

    int whoIsEnemy(const int &team) {
        if (home == team)
            return away;
        return home;
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

    void swapTeams(int &teamA, int &teamB) {

        if (meetingContains(teamA) && meetingContains(teamB)) {
            if (isHome(teamA)) {
                home = teamB;
                away = teamA;
            } else {
                home = teamA;
                away = teamB;
            }
        } else {

            bool recently_swapped = false;

            if (meetingContains(teamA)) {
                if (isHome(teamA)) {
                    home = teamB;
                } else {
                    away = teamB;
                }
                recently_swapped = true;
            }
            if (meetingContains(teamB) and !recently_swapped) {
                if (isHome(teamB)) {
                    home = teamA;
                } else {
                    away = teamA;
                }
            }
        }
    }
};

typedef std::vector<Meeting> tMeetings;
typedef std::vector<tMeetings> tSchedule;