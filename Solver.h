#pragma once
#include "Problem.h"

class Solver {
public:
    virtual ~Solver();
    void initiateRandomSolution(int seed);

    Problem mProblem;
    std::vector<Solution> mSolutions;


};