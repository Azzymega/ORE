#include "stdio.hpp"
#include "../libk/Display.hpp"
#include "../libReflector/Reflector.h"

extern Display Console;

void printf(const char* Message) {

    Console.Write(Message);
    
}

void puts(const char* Message) {

    printf(Message);

}

void gprintf(GUID Address) {

    Console.Write(Address);

}

void printi(int Value) {

    char Buffer[35];

    printf(itoa(Value,Buffer,10));
}