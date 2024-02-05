#pragma once
#include "../../libc/string.hpp"
#include "../Base.h"

struct Scanner {
    void* Binary;
    u4 Pointer;

    void Initialize(void* Binary);

    u1 GetB();
    u2 GetS();
    u4 GetI();
    void RawSet(void* Address, size_t Count);

    void Rewind(u4 Count);
};

