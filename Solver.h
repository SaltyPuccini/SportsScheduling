#pragma once
#include "Problem.h"

class Solver {
public:
    virtual ~Solver();
    void initiateSolution();

    Problem mProblem;
    Solution mSolution;
};