#pragma once
#include "IConstraint.h"

class BasicConstraint : public IConstraint {
public:
    BasicConstraint(const std::vector<int> teams, const std::vector<int> slots);


    std::vector<int> mTeams;
    std::vector<int> mSlots;

    // Destructor
    ~BasicConstraint();

    // Violation check
    bool isViolated(Solution &solution) const override;
};
