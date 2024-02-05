#pragma once
#include "../libReflector/Reflector.h"

struct GUID;

extern "C" void printf(const char* Message);
extern "C" void puts(const char* Message);

// EXTENSIONS

extern "C" void gprintf(GUID Address);
extern "C" void printi(int Value);