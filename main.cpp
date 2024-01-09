#include <fstream>
#include "FileParser/FileParser.h"
#include "Solver.h"

int main() {

    Solver solver;

    FileParser parser;
    parser.parse("../Late4.xml", "../w.weights", "../SA.config", solver.mProblem);

    for (int i = 0; i < 3; i++) {
        solver.initiateGlobalRNG(0);
        solver.anneal();
        std::cout << solver.mSolutions[0].mHardViolation << std::endl;
        std::ofstream file("../Late4_results_" + std::to_string(i) + ".csv");

        // Write CSV header
        file << "overallLoop,current,overallBest\n";

        // Write data
        for (int j = 0; j < solver.mCurr.size(); j++) {
            file << j << "," << solver.mCurr[j] << "," << solver.mBest[j] << "\n";
        }
        file.close();


        solver.mCurr.clear();
        solver.mBest.clear();
        solver.mSolutions.clear();

    }


    return 0;
}
