cmake_minimum_required(VERSION 3.10)

set(PROJECT_N Project6)
project(${PROJECT_N} VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_N} src/main.cpp lib/hack/Parser.cpp lib/hack/Assembler.cpp lib/hack/SymbolTable.cpp)

target_include_directories(${PROJECT_N} PUBLIC lib)
