#pragma once

#include <memory>
#include "Constraints/IConstraint.h"

class Problem {
public:
    Problem(std::vector<int> teams, std::vector<int> slots, std::vector<std::shared_ptr<IConstraint>> constraints);

    std::vector<std::shared_ptr<IConstraint>> mConstraints;
private:
    std::vector<int> mTeams;
    std::vector<int> mSlots;
};