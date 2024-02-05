cmake_minimum_required(VERSION 3.25)
project(Loader)

set(CMAKE_CXX_STANDARD 17)

add_library(Marx STATIC Engine.cpp Native.cpp)

#add_subdirectory(Test)