#include <fstream>
#include <iomanip>
#include <chrono>
#include "FileParser/FileParser.h"
#include "SimulatedAnnealing/Solver.h"

int main(int argc, char *argv[]) {

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <XML file> <config file> <run number>\n";
        return 1;
    }

    std::string xmlFile = argv[1];
    std::string configFile = argv[2];
    int runCount;

    runCount = std::stoi(argv[3]);


    size_t dotPositionXML = xmlFile.rfind('.');
    std::string nameOnlyXML = xmlFile.substr(0, dotPositionXML);

    size_t dotPositionConfig = xmlFile.rfind('.');
    std::string nameOnlyConfig = xmlFile.substr(0, dotPositionConfig);

    Solver solver;
    FileParser parser;
    parser.parse(xmlFile, configFile, solver.mProblem);

    for (int i = 0; i < runCount; i++) {
        solver.initiateGlobalRNG(i);

        auto start = std::chrono::high_resolution_clock::now();
        solver.anneal();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        parser.saveResults(xmlFile, configFile, i, solver);
        std::cout << solver.mSolution.mFitness << std::endl;
        std::cout << "Is basic violated? " << solver.mProblem.mConstraints[0]->isViolated(solver.mSolution)
                  << std::endl;
        if (solver.mProblem.mIsPhased) {
            std::cout << "Is phased violated? " << solver.mProblem.mConstraints[1]->isViolated(solver.mSolution)
                      << std::endl;
        }
        std::cout << "Soft violations: " << solver.countSoftViolations()<< std::endl;
        std::cout << "Hard violations: " << solver.countHardViolations()<< std::endl;
        std::cout <<"Elapsed: "<<elapsed<< "[s]."<<std::endl;
        std::cout << std::endl;
        solver.clearArchive();
    }


    return 0;
}
