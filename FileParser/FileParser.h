#pragma once

#include <sstream>
#include <iostream>
#include <memory>
#include "pugiXML/pugixml.hpp"
#include "../Constraints/GA/ConstraintGA1.h"
#include "../Constraints/BR/ConstraintBR1.h"
#include "../Constraints/BR/ConstraintBR2.h"
#include "../Constraints/SE1/ConstraintSE1.h"
#include "../Constraints/FA/ConstraintFA2.h"
#include "../Constraints/CA/ConstraintCA1.h"
#include "../Constraints/CA/ConstraintCA2.h"
#include "../Constraints/CA/ConstraintCA3.h"
#include "../Constraints/CA/ConstraintCA4.h"
#include "../Constraints/PhasedConstraint.h"
#include "../Constraints/BasicConstraint.h"
#include "../Problem/Problem.h"
#include "../SimulatedAnnealing/Solver.h"

class FileParser {

    void parseSlots(std::vector<int> &slots, const pugi::xml_document &doc);

    void parseTeams(std::vector<int> &teams, const pugi::xml_document &doc);

    void parseConstraints(std::vector<std::shared_ptr<IConstraint>> &constraints, const pugi::xml_document &doc);

    void parsePhased(const pugi::xml_document &doc, std::vector<std::shared_ptr<IConstraint>> &constraints,
                     std::vector<int> teams, std::vector<int> slots);

public:
    bool mIsPhased;

    std::vector<int> parseConfig(const std::string &weightFile);

    void parseXML(const std::string &filename, Problem &problem);

    void parse(const std::string &filename, const std::string &SAFile, Problem &problem);

    bool parseSAConfig(const std::string &filename, paramsSA &params, std::vector<int> &intVector);

    void
    saveResults(const std::string &nameOnlyXML, const std::string &nameOnlyConfig, int runNumber, const Solver &solver);
};