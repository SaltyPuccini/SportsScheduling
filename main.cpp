#include "XMLParser/XMLParser.h"
#include "Solver.h"

int main() {

    Solver solver;

    XMLParser parser;
    parser.parse("../Late4.xml", "../constraints.config", "../SA.config", solver.mProblem);

    solver.anneal();
    std::cout << solver.mSolutions[0].mFitness << std::endl;
    std::cout << solver.mProblem.mConstraints[0]->isViolated(solver.mSolutions[0]) << std::endl;

    return 0;
}
