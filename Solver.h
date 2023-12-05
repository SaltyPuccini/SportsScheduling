#pragma once
#include "Problem.h"

class Solver {
public:
    virtual ~Solver();
    void initiateRandomSolution();
    void setSeed(int seed);
    void anneal();
    void swapRounds(Solution &solution);
    void swapTeams(Solution &solution);
    void swapHomes(Solution &solution);

    Problem mProblem;
    std::vector<Solution> mSolutions;
    int mSeed;


};