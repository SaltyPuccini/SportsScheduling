#include "XMLParser/XMLParser.h"
#include "Solver.h"

int main() {

    Solver solver;

    XMLParser parser;
    parser.parse("../early1.xml", "../constraints.config", solver.mProblem);

    for (int i = 0 ; i < 10; i++){
        solver.setSeed(i);
        solver.initiateRandomSolution();
        for (auto constraint: solver.mProblem.mConstraints) {
            constraint->isViolated(solver.mSolutions[i]);
        }
        std::cout<<solver.mSolutions[i].mFitness<<std::endl;
    }

    return 0;
}
