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

    size_t dotPositionConfig = configFile.rfind('.');
    std::string nameOnlyConfig = configFile.substr(0, dotPositionConfig);

    Solver solver;
    FileParser parser;
    parser.parse(xmlFile, configFile, solver.mProblem);

    for (int i = 0; i < runCount; i++) {
        solver.initiateGlobalRNG(i);

        auto start = std::chrono::high_resolution_clock::now();
        solver.anneal();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::string resultsFileTXT =
                "random_metaresults" + nameOnlyXML + "_" + nameOnlyConfig + "_" + std::to_string(i) + ".txt";

        if (solver.mProblem.mParams.isAdaptive){
            parser.saveResultsCSV(xmlFile, configFile, i, solver);
            resultsFileTXT =
                    "metaresults" + nameOnlyXML + "_" + nameOnlyConfig + "_" + std::to_string(i) + ".txt";

        }else{
            parser.saveResultsCSV("random_" + xmlFile, configFile, i, solver);
        }




        std::ofstream file(resultsFileTXT);
        if (file.is_open()) {
            std::cout << solver.mSolution.mFitness << std::endl;
            file << solver.mSolution.mFitness << std::endl;
            auto basicViolations = solver.mProblem.mConstraints[0]->isViolated(solver.mSolution);

            std::cout << "Is basic violated? " << basicViolations << std::endl;
            file << "Is basic violated? " << basicViolations << std::endl;

            if (solver.mProblem.mIsPhased) {
                auto phasedViolations = solver.mProblem.mConstraints[1]->isViolated(solver.mSolution);
                std::cout << "Is phased violated? " << phasedViolations << std::endl;
                file << "Is phased violated? " << phasedViolations << std::endl;
            }
            auto softViolations = solver.countSoftViolations();
            std::cout << "Soft violations: " << softViolations << std::endl;
            file << "Soft violations: " << softViolations << std::endl;

            auto hardViolations = solver.countHardViolations();

            std::cout << "Hard violations: " << hardViolations << std::endl;
            file << "Hard violations: " << hardViolations << std::endl;

            std::cout << "Elapsed: " << elapsed << "[s]." << std::endl;
            file << "Elapsed: " << elapsed << "[s]." << std::endl;

            std::cout << std::endl;
            file.close();
        } else {
            std::cerr << "Nie można otworzyć pliku do zapisu." << std::endl;
        }
        solver.clearArchive();
    }


    return 0;
}
