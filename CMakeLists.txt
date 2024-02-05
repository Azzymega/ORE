cmake_minimum_required(VERSION 3.15)
project(entity)

set(CMAKE_CXX_FLAGS "--target=i686-pc-none-elf -fno-rtti -mno-red-zone -fno-exceptions -fno-stack-protector -march=i686 -Wall -Ofast -Wpedantic -g -Wextra -Wdeprecated -ffreestanding -nostdlib")

add_subdirectory(source/libc)
add_subdirectory(source/libk)
add_subdirectory(source/libReflector)
add_subdirectory(source/libDraco)
add_subdirectory(source/libMarx)