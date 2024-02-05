#include "assert.hpp"
#include "stdio.hpp"
#include "../libk/Display.hpp"


extern Display Console;

void assert(bool Result, const char* Message) {

    if (Result) {

        printf(Message);
        printf("\n");
        printf("PANIC!!! SYSTEM HALTED!");
        asm volatile("hlt");
        
    }

    return;
}
