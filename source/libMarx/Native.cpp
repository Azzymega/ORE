#include "../libc/stdio.hpp"
#include "Native.h"

void peekL(long Address, long Value) {

    /*

    printf("[Peek] Writing Long: ");
    printi(Value);
    printf("  to address: ");
    printi(Address);
    printf("\n");

    */

    long* Pointer = reinterpret_cast<long *>(Address);

    *Pointer = Value;

}

void peekI(long Address, int Value) {

    /*

    printf("[Peek] Writing Integer: ");
    printi(Value);
    printf("  to address: ");
    printi(Address);
    printf("\n");

    */

    int* Pointer = reinterpret_cast<int *>(Address);

    *Pointer = Value;

}

void peekS(long Address, short Value) {



    printf("[Peek] Writing Short: ");
    printi(Value);
    printf("  to address: ");
    printi(Address);
    printf("\n");



    short* Pointer = reinterpret_cast<short *>(Address);

    *Pointer = Value;

}

void peekB(long Address, char Value) {

    /*

    printf("[Peek] Writing Char/Byte: ");
    printi(Value);
    printf("  to address: ");
    printi(Address);
    printf("\n");

    */

    char* Pointer = reinterpret_cast<char *>(Address);

    *Pointer = Value;

}
