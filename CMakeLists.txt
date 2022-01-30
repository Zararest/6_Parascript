cmake_minimum_required(VERSION 3.0.0)
project(Lang VERSION 0.1.0)

set(LEXER_DIR ./src/Lexer/)
set(PARCER_DIR ./src/Parcer/)
set(COMPILER_DIR ./src/Compiler/)

add_executable(Compiler ${LEXER_DIR}Lexer.cpp 
                        ${LEXER_DIR}Icontent_of_statement.cpp
                        ${LEXER_DIR}Icontrol_struct.cpp
                        ${LEXER_DIR}Ioperand.cpp
                        ${LEXER_DIR}Ioperator.cpp
                        ${PARCER_DIR}Parcer.cpp
                        ${COMPILER_DIR}Compiler.cpp
                        ${COMPILER_DIR}Asm_request.cpp
                        ${COMPILER_DIR}Statements_request.cpp
                        main.cpp)

add_executable(Lexer main.cpp src/Lexer/Lexer.cpp src/Lexer/Icontent_of_statement.cpp
src/Lexer/Ioperand.cpp src/Lexer/Icontrol_struct.cpp src/Lexer/Ioperator.cpp
src/Parcer/Parcer.cpp)
add_executable(Parcer main.cpp src/Parcer/headers/Parcer.h src/Parcer/Parcer.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
