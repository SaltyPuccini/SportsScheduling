#pragma once
#include "Problem.h"
#include <random>
#include <unordered_set>

class Solver {
public:


    virtual ~Solver();
    void initiateRandomSolution();
    void anneal();

    void initiateGlobalRNG(int seed);
    void partialSwapRounds(Solution &solution);

    void evaluate(Solution &solution);

    void swapHomes(Solution &solution);
    void swapRounds(Solution &solution);
    void swapTeams(Solution &solution);

    Problem mProblem;
    std::vector<Solution> mSolutions;
    int mSeed;
    std::mt19937 gen;
    std::vector<float> mCurr;
    std::vector<float> mBest;



    void partialSwapTeams(Solution &solution, bool phased);
};