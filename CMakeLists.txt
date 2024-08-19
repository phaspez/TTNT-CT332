cmake_minimum_required(VERSION 3.27)
project(TTNT)

set(CMAKE_CXX_STANDARD 17)

add_executable(TTNT main.cpp
        PourWater/node.cpp
        PourWater/bucketState.cpp
        PourWater/bucketState.h
        PourWater/node.h
)
