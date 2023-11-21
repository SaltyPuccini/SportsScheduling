#pragma once
#include "Problem.h"

class Solver {
public:
    Solver(Problem problem);
    virtual ~Solver();

private:
    Problem mProblem;

};