#include "ConstraintSE1.h"

ConstraintSE1::ConstraintSE1(const Mode mode1, int min, int penalty, const std::vector<int> teams,
                             const ConstraintType type)
        : mMode1(mode1), mMin(min), mPenalty(penalty), mTeams(teams), mType(type) {}

ConstraintSE1::~ConstraintSE1() {
}

bool ConstraintSE1::isViolated(Solution &solution) const {
    int numberOfViolations = 0;
    for (int i = 0; i < mTeams.size(); i++) {
        for (int j = i + 1; j < mTeams.size(); j++) {

            int firstMeetingId = -1;
            int separation = -1;

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
            if (separation < mMin) {
                numberOfViolations++;
            }
        }
    }

    if (numberOfViolations == 0) {
        return false;
    } else {

        int multiplier = mSoft;
        if (mType == HARD) {
            multiplier = mHard;
        }

        solution.mFitness += numberOfViolations * (mPenalty * multiplier);
        return true;
    }
}

bool ConstraintSE1::isHard() const {
    if (mType == HARD)
        return true;
    return false;
}