#pragma once
#include "../libc/stdlib.hpp"

#define BadPointer 0xFFFFFFFFFF
#define MB 1048576
#define BlockSize 8
#define GUIDSize 40
#define MaxArraySize 10000

using Object = uint64_t;
using Byte = uint8_t;
