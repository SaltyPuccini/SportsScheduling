#include "XMLParser/XMLParser.h"
#include "Solver.h"

int main() {

        Solver solver;
        solver.setSeed(1);
        XMLParser parser;
        parser.parseXML("../early1.xml", solver.mProblem);
        solver.initiateRandomSolution();

        for (auto constraint:solver.mProblem.mConstraints){
            constraint->isViolated(solver.mSolutions[0]);
        }

    return 0;
}
