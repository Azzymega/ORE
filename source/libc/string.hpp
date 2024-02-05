#pragma once
#include "stdlib.hpp"

extern "C" void* memcpy(void* dest, const void* src, size_t n);
extern "C" int memcmp(const void* s1, const void* s2, size_t n);
extern "C" void* memmove(void* s1, const void* s2, size_t n);
extern "C" void* memset(char* dest, int z, size_t n);
extern "C" size_t strlen(const char* string);
extern "C" int strcmp(const char* firstString, const char* secondString);