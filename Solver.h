#pragma once
#include "Problem.h"
#include <random>
#include <unordered_set>

class Solver {
public:


    virtual ~Solver();
    void initiateRandomSolution();
    void anneal();

    void fixPhase(Solution &solution);

    void partialSwapRounds(Solution &solution);
    void partialSwapTeams(Solution &solution);
    void partialSwapTeamsPhased(Solution &solution);

    void evaluate(Solution &solution);

    void swapHomes(Solution &solution);
    void swapRounds(Solution &solution);
    void swapTeams(Solution &solution);

    Problem mProblem;
    std::vector<Solution> mSolutions;
    int mSeed;
    bool isPhased;
    std::mt19937 gen;


    void teamsPlayOnlyInTs(Solution &solution, int ri, int rj, std::unordered_set<int> &Ts);

    bool CanSwapTeams(const Solution &solution, int ti, int tj, const std::vector<int> &Rs);
};