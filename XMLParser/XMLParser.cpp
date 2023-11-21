#include "XMLParser.h"
#include "../Constraints/BR/ConstraintBR2.h"
#include "../Constraints/SE1/ConstraintSE1.h"
#include "../Constraints/FA/ConstraintFA2.h"
#include "../Constraints/CA/ConstraintCA1.h"
#include "../Constraints/CA/ConstraintCA2.h"
#include "../Constraints/CA/ConstraintCA3.h"
#include "../Constraints/CA/ConstraintCA4.h"

void parseLocalSlots(std::vector<int> &slots, pugi::xml_node constraintNode) {
    std::string slotsStr = constraintNode.attribute("slots").as_string();
    std::stringstream slotsStream(slotsStr);
    int slot;
    while (slotsStream >> slot) {
        slots.push_back(slot);
        if (slotsStream.peek() == ';') {
            slotsStream.ignore();
        }
    }
}

void parseLocalTeams(std::vector<int> &teams, pugi::xml_node constraintNode, const char *teamsVariant) {
    std::string teamsStr = constraintNode.attribute(teamsVariant).as_string();
    std::stringstream teamsStream(teamsStr);
    int team;
    while (teamsStream >> team) {
        teams.push_back(team);
        if (teamsStream.peek() == ';') {
            teamsStream.ignore();
        }
    }
}

void parseLocalMode(Mode &modeType, pugi::xml_node constraintNode, const char *modeVariant) {
    std::string modeStr = constraintNode.attribute(modeVariant).as_string();
    std::stringstream modeStream(modeStr);
    std::string type;
    while (modeStream >> type) {
        if (type == "H") {
            modeType = H;
        } else if (type == "A") {
            modeType = A;
        } else if (type == "HA") {
            modeType = HA;
        } else if (type == "LEQ") {
            modeType = LEQ;
        } else if (type == "EVERY") {
            modeType = EVERY;
        } else if (type == "GLOBAL") {
            modeType = GLOBAL;
        }else if (type == "SLOTS") {
            modeType = SLOTS;
        }
    }
}

void parseLocalConstraintType(ConstraintType &constraintType, pugi::xml_node constraintNode) {
    std::string typeStr = constraintNode.attribute("type").as_string();
    std::stringstream typeStream(typeStr);
    std::string type;
    while (typeStream >> type) {
        constraintType = SOFT;
        if (type == "HARD") {
            constraintType = HARD;
        }
    }
}

void parseLocalMeetings(tMeetings &meetings, pugi::xml_node constraintNode) {
    std::string meetingStr = constraintNode.attribute("meetings").as_string();
    std::stringstream meetingStream(meetingStr);

    std::string meetingPair;
    while (std::getline(meetingStream, meetingPair, ';')) {
        std::stringstream pairStream(meetingPair);
        std::string homeAway;
        std::vector<int> homeAwayVec;
        while (std::getline(pairStream, homeAway, ',')) {
            homeAwayVec.push_back(std::stoi(homeAway));
        }
        if (homeAwayVec.size() == 2) {
            meetings.push_back({homeAwayVec[0], homeAwayVec[1]});
        }
    }
}



void parseGameConstraints(const pugi::xml_node &gameConstraintsNode, std::vector<std::shared_ptr<IConstraint>> &constraints) {
    for (pugi::xml_node constraintNode = gameConstraintsNode.child(
            "GA1"); constraintNode; constraintNode = constraintNode.next_sibling("GA1")) {

        int max = constraintNode.attribute("max").as_int();
        int min = constraintNode.attribute("min").as_int();
        int penalty = constraintNode.attribute("penalty").as_int();

        //Parse slots
        std::vector<int> slots;
        parseLocalSlots(slots, constraintNode);

        //Parse ConstraintType
        ConstraintType constraintType;
        parseLocalConstraintType(constraintType, constraintNode);

        //Parse Meeting
        tMeetings meetings;
        parseLocalMeetings(meetings, constraintNode);

        // Create ConstraintGA1 object and add to constraints vector
        constraints.push_back(std::make_shared<ConstraintGA1>(max, meetings, min, penalty, slots, constraintType));
    }
}

void parseBreakConstraints(const pugi::xml_node &breakConstraintsNode, std::vector<std::shared_ptr<IConstraint>> &constraints) {
    //BR1
    for (pugi::xml_node constraintNode = breakConstraintsNode.child(
            "BR1"); constraintNode; constraintNode = constraintNode.next_sibling("BR1")) {

        int intp = constraintNode.attribute("intp").as_int();
        int penalty = constraintNode.attribute("penalty").as_int();
        int team = constraintNode.attribute("teams").as_int();

        //Parse slots
        std::vector<int> slots;
        parseLocalSlots(slots, constraintNode);

        //Parse ConstraintType
        ConstraintType constraintType;
        parseLocalConstraintType(constraintType, constraintNode);

        //Parse mode1
        Mode mode1;
        parseLocalMode(mode1, constraintNode, "mode1");

        //Parse mode2
        Mode mode2;
        parseLocalMode(mode2, constraintNode, "mode2");

        // Create ConstraintBR1 object and add to constraints vector
        constraints.push_back(
                std::make_shared<ConstraintBR1>(intp, mode1, mode2, penalty, slots, team, constraintType));
    }

    //BR2
    for (pugi::xml_node constraintNode = breakConstraintsNode.child(
            "BR2"); constraintNode; constraintNode = constraintNode.next_sibling("BR2")) {

        int intp = constraintNode.attribute("intp").as_int();
        int penalty = constraintNode.attribute("penalty").as_int();

        //Parse slots
        std::vector<int> slots;
        parseLocalSlots(slots, constraintNode);

        //Parse teams
        std::vector<int> teams;
        parseLocalTeams(teams, constraintNode, "teams");

        //Parse ConstraintType
        ConstraintType constraintType;
        parseLocalConstraintType(constraintType, constraintNode);

        //Parse homeMode
        Mode homeMode;
        parseLocalMode(homeMode, constraintNode, "homeMode");

        Mode mode2;
        parseLocalMode(mode2, constraintNode, "mode2");

        // Create ConstraintBR2 object and add to constraints vector
        constraints.push_back(
                std::make_shared<ConstraintBR2>(intp, homeMode, mode2, penalty, slots, teams, constraintType));
    }
}

void parseSeparationConstraints(const pugi::xml_node &breakConstraintsNode, std::vector<std::shared_ptr<IConstraint>> &constraints) {
    for (pugi::xml_node constraintNode = breakConstraintsNode.child(
            "SE1"); constraintNode; constraintNode = constraintNode.next_sibling("SE1")) {

        int min = constraintNode.attribute("min").as_int();
        int penalty = constraintNode.attribute("penalty").as_int();

        //Parse teams
        std::vector<int> teams;
        parseLocalTeams(teams, constraintNode, "teams");

        //Parse ConstraintType
        ConstraintType constraintType;
        parseLocalConstraintType(constraintType, constraintNode);

        //Parse mode1
        Mode mode1;
        parseLocalMode(mode1, constraintNode, "mode1");

        // Create ConstraintSE1 object and add to constraints vector
        constraints.push_back(
                std::make_shared<ConstraintSE1>(mode1, min, penalty, teams, constraintType));
    }
}

void parseFairnessConstraints(const pugi::xml_node &breakConstraintsNode, std::vector<std::shared_ptr<IConstraint>> &constraints) {
    for (pugi::xml_node constraintNode = breakConstraintsNode.child(
            "FA2"); constraintNode; constraintNode = constraintNode.next_sibling("FA2")) {

        int intp = constraintNode.attribute("intp").as_int();
        int penalty = constraintNode.attribute("penalty").as_int();

        //Parse slots
        std::vector<int> slots;
        parseLocalSlots(slots, constraintNode);

        //Parse teams
        std::vector<int> teams;
        parseLocalTeams(teams, constraintNode, "teams");

        //Parse ConstraintType
        ConstraintType constraintType;
        parseLocalConstraintType(constraintType, constraintNode);

        //Parse mode
        Mode mode;
        parseLocalMode(mode, constraintNode, "mode");

        // Create ConstraintFA2 object and add to constraints vector
        constraints.push_back(
                std::make_shared<ConstraintFA2>(intp, mode, penalty, slots, teams, constraintType));
    }
}

void parseCapacityConstraints(const pugi::xml_node &breakConstraintsNode, std::vector<std::shared_ptr<IConstraint>> &constraints){
    //CA1
    for (pugi::xml_node constraintNode = breakConstraintsNode.child(
            "CA1"); constraintNode; constraintNode = constraintNode.next_sibling("CA1")) {

        int max = constraintNode.attribute("max").as_int();
        int min = constraintNode.attribute("min").as_int();
        int penalty = constraintNode.attribute("penalty").as_int();
        int team = constraintNode.attribute("teams").as_int();

        //Parse slots
        std::vector<int> slots;
        parseLocalSlots(slots, constraintNode);

        //Parse ConstraintType
        ConstraintType constraintType;
        parseLocalConstraintType(constraintType, constraintNode);

        //Parse mode
        Mode mode;
        parseLocalMode(mode, constraintNode, "mode");

        // Create ConstraintCA1 object and add to constraints vector
        constraints.push_back(
                std::make_shared<ConstraintCA1>(max, min, mode, penalty, slots, team, constraintType));
    }

    //CA2
    for (pugi::xml_node constraintNode = breakConstraintsNode.child(
            "CA2"); constraintNode; constraintNode = constraintNode.next_sibling("CA2")) {

        int max = constraintNode.attribute("max").as_int();
        int min = constraintNode.attribute("min").as_int();
        int penalty = constraintNode.attribute("penalty").as_int();
        int teams1 = constraintNode.attribute("teams1").as_int();

        //Parse teams
        std::vector<int> teams2;
        parseLocalTeams(teams2, constraintNode, "teams2");

        //Parse slots
        std::vector<int> slots;
        parseLocalSlots(slots, constraintNode);

        //Parse ConstraintType
        ConstraintType constraintType;
        parseLocalConstraintType(constraintType, constraintNode);

        //Parse mode
        Mode mode1;
        parseLocalMode(mode1, constraintNode, "mode1");

        Mode mode2;
        parseLocalMode(mode2, constraintNode, "mode2");

        // Create ConstraintCA2 object and add to constraints vector
        constraints.push_back(
                std::make_shared<ConstraintCA2>(max, min, mode1, mode2, penalty, slots, teams1, teams2, constraintType));
    }

    //CA3
    for (pugi::xml_node constraintNode = breakConstraintsNode.child(
            "CA3"); constraintNode; constraintNode = constraintNode.next_sibling("CA3")) {

        int max = constraintNode.attribute("max").as_int();
        int min = constraintNode.attribute("min").as_int();
        int penalty = constraintNode.attribute("penalty").as_int();
        int intp = constraintNode.attribute("intp").as_int();

        //Parse teams
        std::vector<int> teams1;
        parseLocalTeams(teams1, constraintNode, "teams1");

        //Parse teams
        std::vector<int> teams2;
        parseLocalTeams(teams2, constraintNode, "teams2");

        //Parse ConstraintType
        ConstraintType constraintType;
        parseLocalConstraintType(constraintType, constraintNode);

        //Parse mode
        Mode mode1;
        parseLocalMode(mode1, constraintNode, "mode1");

        Mode mode2;
        parseLocalMode(mode2, constraintNode, "mode2");

        // Create ConstraintCA3 object and add to constraints vector
        constraints.push_back(
                std::make_shared<ConstraintCA3>(intp, max, min, mode1, mode2, penalty, teams1, teams2, constraintType));
    }

    //CA4
    for (pugi::xml_node constraintNode = breakConstraintsNode.child(
            "CA4"); constraintNode; constraintNode = constraintNode.next_sibling("CA4")) {

        int max = constraintNode.attribute("max").as_int();
        int min = constraintNode.attribute("min").as_int();
        int penalty = constraintNode.attribute("penalty").as_int();

        //Parse teams
        std::vector<int> teams1;
        parseLocalTeams(teams1, constraintNode, "teams1");

        //Parse teams
        std::vector<int> teams2;
        parseLocalTeams(teams2, constraintNode, "teams2");

        //Parse slots
        std::vector<int> slots;
        parseLocalSlots(slots, constraintNode);

        //Parse ConstraintType
        ConstraintType constraintType;
        parseLocalConstraintType(constraintType, constraintNode);

        //Parse mode
        Mode mode1;
        parseLocalMode(mode1, constraintNode, "mode1");

        Mode mode2;
        parseLocalMode(mode2, constraintNode, "mode2");

        // Create ConstraintCA3 object and add to constraints vector
        constraints.push_back(
                std::make_shared<ConstraintCA4>(max, min, mode1, mode2, penalty, slots, teams1, teams2, constraintType));
    }


}



void XMLParser::parseTeams(std::vector<int> &teams, const pugi::xml_document &doc) {
    pugi::xml_node teamsNode = doc.child("Instance").child("Resources").child("Teams");
    for (pugi::xml_node team = teamsNode.child("team"); team; team = team.next_sibling("team")) {
        int teamID = team.attribute("id").as_int();
        teams.push_back(teamID);
    }

}

void XMLParser::parseSlots(std::vector<int> &slots, const pugi::xml_document &doc) {
    pugi::xml_node slotsNode = doc.child("Instance").child("Resources").child("Slots");
    for (pugi::xml_node slot = slotsNode.child("slot"); slot; slot = slot.next_sibling("slot")) {
        int slotID = slot.attribute("id").as_int();
        slots.push_back(slotID);
    }
}

void XMLParser::parseConstraints(std::vector<std::shared_ptr<IConstraint>> &constraints, const pugi::xml_document &doc) {

    pugi::xml_node constraintsNode = doc.child("Instance").child("Constraints");

    for (pugi::xml_node constraintNode = constraintsNode.first_child(); constraintNode; constraintNode = constraintNode.next_sibling()) {

        std::string constraintType = constraintNode.name();

        if (constraintType == "GameConstraints") {
            parseGameConstraints(constraintNode, constraints);
        } else if (constraintType == "CapacityConstraints") {
            parseCapacityConstraints(constraintNode, constraints);
        } else if (constraintType == "BreakConstraints") {
            parseBreakConstraints(constraintNode, constraints);
        } else if (constraintType == "FairnessConstraints") {
            parseFairnessConstraints(constraintNode, constraints);
        } else if (constraintType == "SeparationConstraints") {
            parseSeparationConstraints(constraintNode, constraints);
        }
    }
}



Problem *XMLParser::parseXML(const std::string &filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());

    std::vector<int> teams;
    std::vector<int> slots;
    std::vector<std::shared_ptr<IConstraint>> constraints;

    if (result) {
        parseTeams(teams, doc);
        parseSlots(slots, doc);
        parseConstraints(constraints, doc);
    } else {
        std::cerr << "Failed to load XML file: " << result.description() << std::endl;
    }

    return new Problem(teams, slots, constraints);
}


