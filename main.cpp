#include "XMLParser/XMLParser.h"
#include "Solver.h"

int main() {

        Solver solver;
        XMLParser parser;
        parser.parseXML("../instance.xml", solver.mProblem);
        solver.initiateSolution();
        //solver.randomSolution(10);

        for (auto constraint:solver.mProblem.mConstraints){
            constraint->isViolated(solver.mSolutions[0]);
        }




    return 0;
}
