#pragma once

#include <memory>
#include "../Constraints/IConstraint.h"
#include "../SimulatedAnnealing/paramsSA.h"

class Problem {
public:
    Problem();

    std::vector<std::shared_ptr<IConstraint>> mConstraints;
    std::vector<int> mTeams;
    std::vector<int> mSlots;
    bool mIsPhased;
    paramsSA mParams;
};