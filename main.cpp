#include "Problem.h"
#include "XMLParser/XMLParser.h"

int main() {

    Problem *problem = (new XMLParser)->parseXML("../instance.xml");
    delete problem;

    return 0;
}
