#pragma once
#include "../../libDraco/Loader.h"
#include "../Reflector.h"
#include "../../libc/string.hpp"

struct ObjectType : Property {
    GUID Class;
};

struct Class : Property {
    ClassFile File;

    PElement* GetPE(Object Index);
    Method* GetMethod(char* Descriptor, char* ClassName, char* SDescriptor);
    Method* GetMethod(char* Descriptor, char* SDescriptor);
    Field* GetField(char* Descriptor, char* ClassName, char* SDescriptor);
    Field* GetField(char* Descriptor, char* SDescriptor);
}
__attribute__((packed));