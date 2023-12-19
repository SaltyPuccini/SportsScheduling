#include "XMLParser/XMLParser.h"
#include "Solver.h"

int main() {

    Solver solver;

    XMLParser parser;
    parser.parse("../Late4.xml", "../constraints.config", "../SA.config", solver.mProblem);

    for (int i = 0 ; i < 1; i++){
        solver.setSeed(i);
        solver.anneal();
        std::cout<<solver.mSolutions[0].mFitness<<std::endl;
        std::cout<<solver.mProblem.mConstraints[0]->isViolated(solver.mSolutions[0])<<std::endl;
//        std::cout<<solver.mProblem.mConstraints[1]->isViolated(solver.mSolutions[0])<<std::endl;

//        for (auto constraint: solver.mProblem.mConstraints) {
//            constraint->isViolated(solver.mSolutions[i]);
//        }
//        std::cout<<solver.mSolutions[i].mFitness<<std::endl;
    }

    return 0;
}
