#pragma once
#include "../Reflector.h"

struct Parts : Property {
    Object Count;
    GUID Addresses[];

    void Set(Object Index, GUID Value);
    GUID Get(Object Index);
    Object GetLength();
};