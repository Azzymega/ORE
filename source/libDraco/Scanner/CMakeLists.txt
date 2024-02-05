cmake_minimum_required(VERSION 3.25)
project(Scanner)
set(CMAKE_CXX_FLAGS "--target=i686-pc-none-elf -fno-stack-protector -march=i686 -Wall -O0 -fPIC -Wpedantic -Wextra -Wdeprecated -ffreestanding -fno-builtin -nostdlib")
set(CMAKE_CXX_STANDARD 17)

add_library(Scanner STATIC Scanner.cpp)