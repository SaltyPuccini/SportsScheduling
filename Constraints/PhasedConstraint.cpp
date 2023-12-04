#include "PhasedConstraint.h"

PhasedConstraint::PhasedConstraint(const std::vector<int> teams, const std::vector<int> slots) : mTeams(teams),
                                                                                                 mSlots(slots) {}
PhasedConstraint::~PhasedConstraint() {}

bool PhasedConstraint::isViolated(Solution &solution) const {
    return false;
}