cmake_minimum_required(VERSION 3.25)
project(data_structures_wet_2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(data_structures_wet_2
        mainWet2.cpp
        recordsCompany.h
        utilesWet2.h
        UnionFind.h
        Record.cpp Record.h
        AVLTree.h
        HashTable.h HashTable.cpp recordsCompany.cpp)
