#include "Problem.h"

Problem::Problem(std::vector<int> teams, std::vector<int> slots, std::vector<std::shared_ptr<IConstraint>> constraints)
        : mTeams(teams), mSlots(slots), mConstraints(constraints) {}
