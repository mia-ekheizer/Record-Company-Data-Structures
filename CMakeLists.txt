cmake_minimum_required(VERSION 3.0.0)
project(wet2 VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(wet2 mainWet2.cpp Costumer.cpp HashTable.cpp Record.cpp recordsCompany.cpp UnionFind.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
