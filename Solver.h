#pragma once
#include "Problem.h"
#include <random>

class Solver {
public:


    virtual ~Solver();
    void initiateRandomSolution();
    void setSeed(int seed);
    void anneal();

    void fixPhase(Solution &solution);
//    void fixBasic(Solution &solution);
//    void partialSwapRounds(Solution &solution);
//    void partialSwapTeams(Solution &solution);
//    void partialSwapTeamsPhased(Solution &solution);

    void evaluate(Solution &solution);

    void swapHomes(Solution &solution);
    void swapRounds(Solution &solution);
    void swapTeams(Solution &solution);

    Problem mProblem;
    std::vector<Solution> mSolutions;
    int mSeed;
    bool isPhased;
    std::mt19937 gen;





};