cmake_minimum_required(VERSION 3.0.0)
project(Lang VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Lang main.cpp)
add_executable(Parcer main.cpp src/Parcer/headers/Parcer.h src/Parcer/Parcer.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
