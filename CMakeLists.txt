cmake_minimum_required(VERSION 3.16)
project(NSGA2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(NSGA2
        GeneticAlgorithm.cpp
        GeneticAlgorithm.h
        Individual.cpp
        Individual.h
        NSGA2.cpp
        pch.cpp
        pch.h
        Population.cpp
        Population.h problems/Problem.h problems/ZDT1.cpp problems/ZDT1.h)
