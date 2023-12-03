#include "ConstraintSE1.h"

// Constructor implementation
ConstraintSE1::ConstraintSE1(const Mode mode1, int min, int penalty, const std::vector<int> teams,
                             const ConstraintType type)
        : mMode1(mode1), mMin(min), mPenalty(penalty), mTeams(teams), mType(type) {}

// Destructor implementation
ConstraintSE1::~ConstraintSE1() {
}

bool ConstraintSE1::isViolated(Solution &solution) const {
    int numberOfViolations = 0;
    for (int i = 0; i < mTeams.size(); i++) {
        for (int j = i+1; j < mTeams.size(); j++) {

            //tutaj mamy wszystkie pary drużyn
            //chcemy znalezc slot dla ich 1 meczu, zapisac, potem slot dla 2 i policzyc roznice

            int firstMeetingId = -1;
            int separation = -1;

            //iterujemy po timeslotach i dla kazdego meetingu w timeslocie okreslamy, czy to jest ten sprawdzany.
            //jestli tak, to zapisujemy licznik petli dla ktorego nastapilo piersze spotkanie i szukamy drugiego.
            //jak mamy, to liczymy seperation i wychodzimy z petli obu

            for (int counter = 0; counter < solution.mSchedule.size() && separation == -1; counter++) {
                for (auto meeting: solution.mSchedule[counter]) {
                    if (meeting.isPartiallyEqual({mTeams[i], mTeams[j]})) {
                        if (firstMeetingId != -1) {
                            separation = counter - firstMeetingId;
                            break;
                        }
                        firstMeetingId = counter;
                    }
                }
            }
            //jak seperation za krótkie, zwiekszamy violation
            if (separation < mMin){
                numberOfViolations++;
            }
        }
    }

    if (numberOfViolations == 0){
        return false;
    }
    else{
        solution.mFitness += numberOfViolations * mPenalty;
        return true;
    }
}
