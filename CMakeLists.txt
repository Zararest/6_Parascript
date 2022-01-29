cmake_minimum_required(VERSION 3.0.0)
project(Lang VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Lexer main.cpp src/Lexer/Lexer.cpp src/Lexer/Icontent_of_statement.cpp
src/Lexer/Ioperand.cpp src/Lexer/Icontrol_struct.cpp src/Lexer/Ioperator.cpp
src/Parcer/Parcer.cpp)
add_executable(Parcer main.cpp src/Parcer/headers/Parcer.h src/Parcer/Parcer.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
