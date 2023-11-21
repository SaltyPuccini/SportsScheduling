#pragma once
#include <sstream>
#include <iostream>
#include <memory>
#include "pugiXML/pugixml.hpp"
#include "../Constraints/GA/ConstraintGA1.h"
#include "../Constraints/BR/ConstraintBR1.h"
#include "../Problem.h"

class XMLParser {

    void parseSlots(std::vector<int> &slots, const pugi::xml_document &doc);

    void parseTeams(std::vector<int> &teams, const pugi::xml_document &doc);

    void parseConstraints(std::vector<std::shared_ptr<IConstraint>> &constraints, const pugi::xml_document &doc);

public:
    void parseXML(const std::string& filename, Problem &problem);
};