#pragma once

#include "../libDraco/Loader.h"
#include "../libReflector/Reflector.h"
#include "OperationCodes.h"

#define ABISpace 12

using u8 = uint64_t;

enum ArrayType : u1 {
    ATBoolean = 4,
    ATChar = 5,
    ATFloat = 6,
    ATDouble = 7,
    ATByte = 8,
    ATShort = 9,
    ATInteger = 10,
    ATLong = 11
};

struct Stack {
    Object AllocatedCount;
    Object Count;
    GUID Values;

    void Push(GUID Value);
    void Pop();
    GUID Top() const;
    GUID TopAndPop();
    void Initialize(u4 Count);
};

struct Locals {
    Object Count;
    GUID Values;

    GUID Get(u4 Index) const;
    void Set(u4 Index, GUID Value) const;
    void Initialize(u4 Count);
};

struct Executive {
    static void Execute(ACode* Code, Object IP, Stack* SP, Locals* Registers, Class* ClassReference);
};
