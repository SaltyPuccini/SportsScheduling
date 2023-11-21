#include "Problem.h"
#include "XMLParser/XMLParser.h"
#include "Solver.h"

int main() {

    Solver solver;
    XMLParser parser;
    parser.parseXML("../instance.xml", solver.mProblem);
    solver.initiateSolution();



    return 0;
}
