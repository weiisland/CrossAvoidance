cmake_minimum_required(VERSION 3.13)
project(CrossAvoidanceAlgo)

set(CMAKE_CXX_STANDARD 14)

add_executable(CrossAvoidanceAlgo main.cpp header/droplet.h header/graph.h header/routing.h routing.cpp graph.cpp)