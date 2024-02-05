#pragma once
#include "Base.h"
#include "../libc/stdio.hpp"
#include "../libc/stdlib.hpp"
#include "../libc/string.hpp"

struct ClassFile;
struct Class;
struct Parts;
struct ObjectType;

enum PropertyType : Object {
    TLocation = 0,
    TPacked = 1,
    TArray = 2,
    TPTClass = 3,
    TCObject = 4
};

struct GUID {
    Byte Internal[40];
}
__attribute__((packed));

struct Property {
    Object Size;
    PropertyType Type;
};

struct Properties {
    Object Count;
    Property Values[];

    Property* Get(Object Index);
};

struct Location : Property {
    Object Experience;
    Object Field;
};

struct Packed : Property {
    Object Experience;
    Object Field;
    Object Length;
};

struct Representation {
    Object Size = 56;
    GUID Identifier;
    Properties Values;

    GUID Initialize(Parts Size);
    GUID Initialize(Location Position);
    GUID Initialize(Packed Position);
    GUID Initialize(Class Class);
    GUID Initialize(ObjectType Type, Parts Members);

    Location* GetLocation();

    Packed* GetPacked();

    // Extensions!

    // Array Extension

    Parts* GetParts();

    // Class Extension

    Class* GetClass();
};

enum State : Byte {
    Used = 25,
    Free = 15
};

enum Type : Byte {
    TMatter,
    TConsciousness
};

struct Consciousness {
    Object Allocated = 16;
    Object Count;
    Representation Reflections[];

    void Initialize();

    bool Check();
    bool Check(Object Length);

    Representation* Get(Object Index);

    Representation* GetLast();

    GUID Add(Location Position);

    GUID AddPacked(Packed Position);

    Location Get(GUID Value);

    Packed* GetPacked(GUID Value);

    // Extensions!

    // Array Extension

    Representation* GetArray(GUID Array);

    GUID AddArray(Object Length);

    // Class Extension

    GUID AddObject(GUID Template, Object Length);

    GUID AddClass(ClassFile File);

    GUID GetClassReference(const char* Name);

    Class* GetClass(GUID Address);

    Class* GetClass(const char* Name);
};

union Value {
    Object Object;
    int Integer;
    long Long;
    float Float;
    double Double;
};

struct Matter {
    State Field[115000];
    Value Objects[115000];

    void Initialize();

    bool Check();
    bool Check(Object Length);

    Location Add(Object Value);

    Location Add(int Value);

    Location Add(long Value);

    Location Add(float Value);

    Location Add(double Value);

    Packed AddPacked(size_t Length);

    Object Get(Location Value);

    int GetI(Location Value);

    long GetL(Location Value);

    float GetF(Location Value);

    double GetD(Location Value);

    void Set(Location Position, Object Value);

    void Set(Location Position, float Value);

    void Set(Location Position, double Value);

    void Set(Location Position, int Value);

    void Set(Location Position, long Value);

    void* GetPacked(Location Value);
};

struct Experience {
    bool Full;
    Object ID;
    State State;
    Type Type;
    union {
        Matter Matter;
        Consciousness Consciousness;
    };
    Experience* Initialize();
    Experience* Initialize(enum Type Type);

    Experience* Check();
    Experience* Check(Object Length);
};

struct World {
    Object Count;
    Experience Experiences[];

    void Initialize(Object Count);

    Experience* Get(Object BlocksCount);
    Experience* Get();

    Experience* GetDefinition(Object Length);
    Experience* GetDefinition();

    GUID Add(Object Value);

    GUID Add(int Value);

    GUID Add(long Value);

    GUID Add(float Value);

    GUID Add(double Value);

    GUID AddPacked(size_t Length);

    Object Get(GUID Identifier);

    int GetI(GUID Identifier);

    long GetL(GUID Identifier);

    float GetF(GUID Identifier);

    double GetD(GUID Identifier);

    void Set(GUID Identifier, Object Value);

    void Set(GUID Identifier, float Value);

    void Set(GUID Identifier, double Value);

    void Set(GUID Identifier, int Value);

    void Set(GUID Identifier, long Value);

    void* GetPacked(GUID Identifier);

    // Extensions!

    // Array Extension

    GUID AddArray(Object Length);

    Representation* GetArray(GUID Array);

    GUID GetArrayValue(GUID Array, Object Index);

    void SetArrayValue(GUID Array, Object Index, GUID Value);

    Object GetArrayLength(GUID Array);

    // Class Extension

    GUID AddClass(ClassFile File);

    Class* GetClass(GUID Address);

    GUID GetClassReference(const char* ClassName);

    Class* GetClass(const char* ClassName);

    GUID AddObject(GUID Template);

    // TODO Implement ObjectGetClass!!!
};

