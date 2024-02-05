#include <stddef.h>
#include "Scanner.h"

u2 Scanner::GetS() {
    u1 Temp[2];
    Temp[0] = ((u1*)(this->Binary))[this->Pointer++];
    Temp[1] = ((u1*)(this->Binary))[this->Pointer++];
    return Temp[0] >> 8 | Temp[1];
}

u1 Scanner::GetB() {
    u1 Temp[1];
    Temp[0] = ((u1*)(this->Binary))[this->Pointer++];
    return Temp[0];
}

u4 Scanner::GetI() {
    u1 Temp[4];
    Temp[0] = ((u1*)(this->Binary))[this->Pointer++];
    Temp[1] = ((u1*)(this->Binary))[this->Pointer++];
    Temp[2] = ((u1*)(this->Binary))[this->Pointer++];
    Temp[3] = ((u1*)(this->Binary))[this->Pointer++];
    return Temp[3] << 0 | Temp[2] << 8 | Temp[1] << 16 | Temp[0] << 24;
}

void Scanner::Rewind(u4 Count) {
    this->Pointer-=Count;
}

void Scanner::Initialize(void *Binary) {
    this->Binary = Binary;
    this->Pointer = 0;
}

void Scanner::RawSet(void *Address, size_t Count) {

    memcpy(Address,(void*)&((u1*)(this->Binary))[this->Pointer],Count);
    Pointer+=Count;

}
