#pragma once
#include "Problem.h"

class Solver {
public:
    virtual ~Solver();
    void initiateSolution();
    void randomSolution(int repetition);

    Problem mProblem;
    std::vector<Solution> mSolutions;
};