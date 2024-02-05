#pragma once

#include "Base.h"
#include "../libReflector/Reflector.h"
#include "Scanner/Scanner.h"

void HaltWithText(const char* Text);
void assert(bool value, const char* Message);

enum OType : u1 {
    TClass = 7,
    TField = 9,
    TMethod = 10,
    TInterfaceMethod = 11,
    TString = 8,
    TInteger = 3,
    TFloat  = 4,
    TLong  = 5,
    TDouble = 6,
    TNAT = 12,
    TUTF = 1
};

enum OFlag : u2 {
    OPublic = 0x0001,
    OFinal = 0x0010,
    OSuper = 0x0020,
    OInterface = 0x0200,
    OAbstract = 0x0400
};

enum OField : u2 {
    OTPublic = 0x0001,
    OTPrivate = 0x0002,
    OTProtected = 0x0004,
    OTStatic = 0x0008,
    OTFinal = 0x0010,
    OTVolatile = 0x0040,
    OTTransient = 0x0080
};

enum OMethod : u2 {
    MTPublic = 0x0001,
    MTPrivate = 0x0002,
    MTProtected = 0x0004,
    MTStatic = 0x0008,
    MTFinal = 0x0010,
    MTSynchronized = 0x0020,
    MTNative = 0x0100,
    MTAbstract = 0x0400
};

struct Attributes;

struct ASource {
    u2 Index;
};

struct AConstant {
    u2 Index;
};

struct AExceptions {
    u2 Number;
    u2 ExceptionTablesIndexes[];
};

struct LocalVariableTable {
    u2 StartPC;
    u2 Length;
    u2 Name;
    u2 Descriptor;
    u2 Index;
};

struct LineNumberTable {
    u2 StartPC;
    u2 LineNumber;
};

struct ALineNumberTable {
    u2 Length;
    LineNumberTable Table[];
};

struct ALocalVariableTable {
    u2 Length;
    LocalVariableTable Table[];
};

struct AExceptionTable {
    u2 StartPC;
    u2 EndPC;
    u2 HandlerPC;
    u2 CatchType;
};

struct ACode {
    u2 Stack;
    u2 Locals;
    u4 Length;
    u1* Code;
    u2 ExceptionTableLength;
    AExceptionTable* ETable;
    Attributes* Values;
};

struct Attribute {
    u2 Name;
    u4 Length;
    union {
        ASource SourceFile;
        AConstant ConstantValue;
        ACode Code;
        AExceptions* Exceptions;
        ALineNumberTable* LNTable;
        ALocalVariableTable* LVTable;
    };
};

struct Attributes {
    u2 Count;
    Attribute Values[];
};

struct PClass {
    u2 Name;
};

struct PFloat {
    GUID Data;
    u4 Bytes;
};

using PInteger = PFloat;

struct PLong {
    GUID Data;
    u4 High;
    u4 Low;
};

using PDouble = PLong;

struct PField {
    u2 Class;
    u2 NAT;
};

struct PNAT {
    u2 Name;
    u2 Descriptor;
};

struct Field {
    OField Flag;
    PNAT Data;
// FIXME
    GUID Static;
    Attributes* Attributes;
};

struct Method {
    OMethod Flag;
    PNAT Data;
    Attributes* Attributes;
};

struct PString {
    u2 UTFIndex;
};

struct PUTF {
    // FIXME
    GUID Address;
    u2 Length;
    char* Bytes;
};

using PMethod = PField;
using PInterfaceMethod = PField;

struct PElement {
    OType Tag;
    union {
        PClass Class;
        PField Field;
        PMethod Method;
        PInterfaceMethod InterfaceMethod;
        PString String;
        PFloat Float;
        PInteger Integer;
        PLong Long;
        PDouble Double;
        PNAT NAT;
        PUTF UTF;
    };

};

struct ConstantPool {
    u2 Count;
    PElement Elements[];
};

struct Interfaces {
    u2 Count;
    u2 Values[];
};

struct Fields {
    u2 Count;
    Field Values[];
};

struct Methods {
    u2 Count;
    Method Values[];
};

struct ClassFile {
    u4 Magic;
    u2 Minor;
    u2 Major;
    ConstantPool* Pool;
    OFlag Flags;
    u2 This;
    u2 Super;
    Interfaces* Interfaces;
    Fields* Fields;
    Methods* Methods;
    Attributes* Attributes;
};

struct Draco {
    Scanner Reader;
    ClassFile* Buffer;

    ClassFile Load(void* Binary);

private:
    ConstantPool* LoadPool();
    Interfaces* LoadInterfaces();
    Fields* LoadFields();
    Methods* LoadMethods();
    Attributes* LoadAttributes();
};