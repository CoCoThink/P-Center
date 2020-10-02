cmake_minimum_required(VERSION 3.15)
project(P_Center)

set(CMAKE_CXX_STANDARD 17)

add_executable(P_Center main.cpp Graph.cpp Graph.h P_Center_Tabu.cpp P_Center_Tabu.h)