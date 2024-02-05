#include "Class.h"

PElement *Class::GetPE(Object Index) {
    return &this->File.Pool->Elements[Index];
}

Method *Class::GetMethod(char *NameDescriptor, char* ClassName, char* TypeDescriptor) {
    Object ClassIndex = -1;
    Object NATIndex = -1;

    char* WantedNameDescriptor = nullptr;
    char* WantedTypeDescriptor = nullptr;

    for (int i = 0; i < this->File.Methods->Count; ++i) {

        Object NameDescriptorIndex = this->File.Methods->Values[i].Data.Name;
        Object TypeDescriptorIndex = this->File.Methods->Values[i].Data.Descriptor;

        WantedNameDescriptor = GetPE(NameDescriptorIndex)->UTF.Bytes;
        WantedTypeDescriptor = GetPE(TypeDescriptorIndex)->UTF.Bytes;

        if (strcmp(NameDescriptor,WantedNameDescriptor) == 0 && strcmp(TypeDescriptor,WantedTypeDescriptor) == 0) {
            return &this->File.Methods->Values[i];
        }
    }

    return nullptr;
}

Field *Class::GetField(char *NameDescriptor, char* ClassName, char* TypeDescriptor) {
    Object ClassIndex = -1;
    Object NATIndex = -1;

    char* WantedNameDescriptor = nullptr;
    char* WantedTypeDescriptor = nullptr;

    for (int i = 0; i < this->File.Fields->Count; ++i) {

        Object NameDescriptorIndex = this->File.Fields->Values[i].Data.Name;
        Object TypeDescriptorIndex = this->File.Fields->Values[i].Data.Descriptor;

        WantedNameDescriptor = GetPE(NameDescriptorIndex)->UTF.Bytes;
        WantedTypeDescriptor = GetPE(TypeDescriptorIndex)->UTF.Bytes;

        if (strcmp(NameDescriptor,WantedNameDescriptor) == 0 && strcmp(TypeDescriptor,WantedTypeDescriptor) == 0) {
            return &this->File.Fields->Values[i];
        }
    }
    assert(true,"[Reflector] failed to find field!");
    return nullptr;
}

Method *Class::GetMethod(char *Descriptor, char *SDescriptor) {
    Object ClassIndex = -1;
    Object NATIndex = -1;

    char* WantedNameDescriptor = nullptr;
    char* WantedTypeDescriptor = nullptr;

    for (int i = 0; i < this->File.Methods->Count; ++i) {

        Object NameDescriptorIndex = this->File.Methods->Values[i].Data.Name;
        Object TypeDescriptorIndex = this->File.Methods->Values[i].Data.Descriptor;

        WantedNameDescriptor = GetPE(NameDescriptorIndex)->UTF.Bytes;
        WantedTypeDescriptor = GetPE(TypeDescriptorIndex)->UTF.Bytes;

        if (strcmp(Descriptor,WantedNameDescriptor) == 0 && strcmp(SDescriptor,WantedTypeDescriptor) == 0) {
            return &this->File.Methods->Values[i];
        }
    }

    return nullptr;
}

Field *Class::GetField(char *Descriptor, char *SDescriptor) {
    Object ClassIndex = -1;
    Object NATIndex = -1;

    char* WantedNameDescriptor = nullptr;
    char* WantedTypeDescriptor = nullptr;

    for (int i = 0; i < this->File.Fields->Count; ++i) {

        Object NameDescriptorIndex = this->File.Fields->Values[i].Data.Name;
        Object TypeDescriptorIndex = this->File.Fields->Values[i].Data.Descriptor;

        WantedNameDescriptor = GetPE(NameDescriptorIndex)->UTF.Bytes;
        WantedTypeDescriptor = GetPE(TypeDescriptorIndex)->UTF.Bytes;

        if (strcmp(Descriptor,WantedNameDescriptor) == 0 && strcmp(SDescriptor,WantedTypeDescriptor) == 0) {
            return &this->File.Fields->Values[i];
        }
    }

    assert(true,"[Reflector] failed to find field!");
    return nullptr;
}
