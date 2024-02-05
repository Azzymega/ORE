cmake_minimum_required(VERSION 3.25)
project(Loader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "--target=i686-pc-none-elf -fno-stack-protector -march=i686 -Wall -O0 -fPIC -Wpedantic -Wextra -Wdeprecated -ffreestanding -fno-builtin -nostdlib")
add_library(Draco STATIC Loader.cpp)

add_subdirectory(Scanner)
#add_subdirectory(Test)

target_link_libraries(Draco PUBLIC Scanner)

#target_link_libraries(LoaderTest PRIVATE Draco)