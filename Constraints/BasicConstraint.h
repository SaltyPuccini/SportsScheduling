#pragma once
#include "IConstraint.h"

class BasicConstraint : public IConstraint {
public:
    BasicConstraint(const std::vector<int> teams, const std::vector<int> slots, ConstraintType constraintType);


    std::vector<int> mTeams;
    std::vector<int> mSlots;
    ConstraintType mType;

    ~BasicConstraint();

    bool isViolated(Solution &solution) const override;
    bool isHard() const override;
};
