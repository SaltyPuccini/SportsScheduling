#pragma once
#include <vector>
#include "Mode.h"
#include "ConstraintType.h"
#include "../Solution/Solution.h"

class IConstraint {
public:
    virtual ~IConstraint() = default;
    virtual bool isViolated(Solution &solution) const = 0;
};