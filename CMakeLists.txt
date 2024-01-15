cmake_minimum_required(VERSION 3.26)
project(Implementacja_AOIN_P)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Implementacja_AOIN_P main.cpp
        Constraints/IConstraint.h
        Constraints/CA/ConstraintCA1.cpp
        Constraints/CA/ConstraintCA1.h
        Constraints/CA/ConstraintCA2.cpp
        Constraints/CA/ConstraintCA2.h
        Constraints/CA/ConstraintCA3.cpp
        Constraints/CA/ConstraintCA3.h
        Constraints/CA/ConstraintCA4.cpp
        Constraints/CA/ConstraintCA4.h
        Constraints/GA/ConstraintGA1.cpp
        Constraints/GA/ConstraintGA1.h
        Constraints/BR/ConstraintBR1.cpp
        Constraints/BR/ConstraintBR1.h
        Constraints/BR/ConstraintBR2.cpp
        Constraints/BR/ConstraintBR2.h
        Constraints/FA/ConstraintFA2.cpp
        Constraints/FA/ConstraintFA2.h
        Constraints/SE1/ConstraintSE1.cpp
        Constraints/SE1/ConstraintSE1.h
        Solution/Solution.cpp
        Solution/Solution.h
        Constraints/ConstraintType.h
        Solution/Meeting.h
        Constraints/Mode.h
        Problem/Problem.h
        SimulatedAnnealing/Solver.cpp
        SimulatedAnnealing/Solver.h
        Problem/Problem.cpp
        Constraints/PhasedConstraint.cpp
        Constraints/PhasedConstraint.h
        SimulatedAnnealing/Solver.cpp
        Constraints/BasicConstraint.cpp
        Constraints/BasicConstraint.h
        SimulatedAnnealing/paramsSA.h
        FileParser/FileParser.cpp
        FileParser/FileParser.h
        FileParser/pugiXML/pugixml.hpp
        FileParser/pugiXML/pugixml.cpp
        FileParser/pugiXML/pugiconfig.hpp
)
