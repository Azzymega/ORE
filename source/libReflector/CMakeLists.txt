cmake_minimum_required(VERSION 3.25)
project(Reflection)

set(CMAKE_CXX_STANDARD 17)

add_library(Reflector STATIC Reflector.cpp Reflector.h Extension/Array.h Extension/Class.h Extension/Array.cpp Extension/Class.cpp)

add_subdirectory(Test)

#target_link_libraries(AllocTest PRIVATE Reflector)
#target_link_libraries(PackedAllocTest PRIVATE Reflector)
#target_link_libraries(GetTest PRIVATE Reflector)
#target_link_libraries(PackedGetTest PRIVATE Reflector)
#target_link_libraries(ArrayTest PRIVATE Reflector)
#target_link_libraries(ClassTest PRIVATE Reflector)
