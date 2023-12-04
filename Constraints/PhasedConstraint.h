#pragma once
#include "IConstraint.h"

class PhasedConstraint : public IConstraint {
public:
    PhasedConstraint(const std::vector<int> teams, const std::vector<int> slots);


    std::vector<int> mTeams;
    std::vector<int> mSlots;

    // Destructor
    ~PhasedConstraint();

    // Violation check
    bool isViolated(Solution &solution) const override;
};
