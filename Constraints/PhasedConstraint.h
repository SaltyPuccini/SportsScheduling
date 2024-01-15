#pragma once
#include "IConstraint.h"

class PhasedConstraint : public IConstraint {
public:
    PhasedConstraint(const std::vector<int> teams, const std::vector<int> slots, ConstraintType constraintType);


    std::vector<int> mTeams;
    std::vector<int> mSlots;
    ConstraintType mType;

    // Destructor
    ~PhasedConstraint();

    // Violation check
    bool isViolated(Solution &solution) const override;
    bool isHard() const override;
};
