#include "Array.h"
#include "../../libc/assert.hpp"

GUID Parts::Get(Object Index) {
    if (this->Count < Index) {
        assert(true,"[Reflector] [ArrayExtension] Bad get array index!!!");
    }
    return this->Addresses[Index];
}

void Parts::Set(Object Index, GUID Value) {
    if (this->Count < Index) {
        assert(true,"[Reflector] [ArrayExtension] Bad set array index!!!");
    }
    this->Addresses[Index] = Value;
}

Object Parts::GetLength() {
    return Count;
}
