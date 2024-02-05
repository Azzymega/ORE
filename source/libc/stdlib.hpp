#pragma once
#include "stddef.h"
#include "stdint.h"

using u1 = uint8_t;
using u2 = uint16_t;
using u4 = uint32_t;
using u8 = uint64_t;

using s1 = int8_t;
using s2 = int16_t;
using s4 = int32_t;
using s8 = int64_t;

using uchar = wchar_t;

extern "C" char *itoa(int value, char *str, int base);
int rand();
void srand(int value);