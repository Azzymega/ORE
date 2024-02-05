cmake_minimum_required(VERSION 3.15)
project(LIBK)

set(CMAKE_CXX_FLAGS "--target=i686-pc-none-elf -fno-stack-protector -march=i686 -Wall -O0 -fPIC -nodefaultlibs -Wpedantic -Wextra -Wdeprecated -ffreestanding -fno-builtin -nostdlib")
add_compile_options("-mno-red-zone")
add_compile_options("-fno-exceptions")
add_compile_options("-fno-rtti")

file(GLOB_RECURSE LIBK
    "*.hpp"
    "*.cpp"
)

add_library(k STATIC ${LIBK}
        UUID.cpp
        UUID.h)