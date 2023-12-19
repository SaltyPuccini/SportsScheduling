#pragma once

#include <memory>
#include "Constraints/IConstraint.h"
#include "paramsSA.h"

class Problem {
public:
    Problem();
    Problem(std::vector<int> teams, std::vector<int> slots, std::vector<std::shared_ptr<IConstraint>> constraints);

    std::vector<std::shared_ptr<IConstraint>> mConstraints;
    std::vector<int> mTeams;
    std::vector<int> mSlots;
    bool mIsPhased;
    paramsSA mParams;
};