#pragma once
#include "../Problem/Problem.h"
#include <random>
#include <unordered_set>

enum NeighbourhoodType {
    Home, Rounds, Teams, PRounds, PTeams, PartialTeamsP
};

class Solver {
public:


    virtual ~Solver();
    void clearArchive();
    void initiateRandomSolution();
    void anneal();

    void initiateGlobalRNG(int seed);
    void partialSwapRounds(Solution &solution);

    void evaluate(Solution &solution);
    int countSoftViolations();
    int countHardViolations();

    void swapHomes(Solution &solution);
    void swapRounds(Solution &solution);
    void swapTeams(Solution &solution);

    Problem mProblem;
    Solution mSolution;
    int mSeed;
    std::mt19937 gen;
    std::vector<float> mBest;
    std::vector<float> mTemperatureArchive;
    std::vector<float> mCurr;
    std::vector<float> mBestFromNew;
    std::vector<float> mAvgFromNew;
    std::vector<float> mWorstFromNew;



    void partialSwapTeams(Solution &solution, bool phased);
};