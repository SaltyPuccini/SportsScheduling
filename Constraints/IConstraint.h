#pragma once
#include <vector>
#include "Mode.h"
#include "ConstraintType.h"
#include "../Solution/Solution.h"

class IConstraint {
public:
    int mSoft;
    int mHard;

    virtual ~IConstraint() = default;
    virtual bool isViolated(Solution &solution) const = 0;
    virtual bool isHard() const = 0;
};