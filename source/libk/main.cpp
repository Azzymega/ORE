#include "stdlib.hpp"
#include "../libc/stdlib.hpp"
#include "../libc/stdio.hpp"
#include "Multiboot.hpp"
#include "Memory.hpp"
#include "../libu/CharacterULIB.hpp"
#include "../libu/DisplayULIB.hpp"
#include "../libu/ObjectULIB.hpp"
#include "../libu/PackedULIB.hpp"
#include "../libu/StringULIB.hpp"
#include "../libu/SystemULIB.hpp"
#include "../libDraco/Loader.h"
#include "../libReflector/Reflector.h"
#include "../libMarx/Engine.h"
#include "../libReflector/Extension/Class.h"

World* Reflector;
MultibootData *Header;
Draco Loader;
extern Display Console;
extern "C" u8 KernelStart;
extern "C" u8 KernelEnd;

extern "C" void kernel_main(u4 MultibootMagic,
                            struct MultibootData *MultibootHeader) {

    Header = MultibootHeader;

    Console = Display();

    char Buffer[15];

    if (MultibootMagic == 0x2BADB002) {

        printf("[KERNEL] Multiboot header is present!\n");
        printf("[KERNEL] Kernel start at: 0x");
        printf(itoa((u8)&KernelStart, Buffer, 16));
        printf("\n[KERNEL] Kernel ends at: 0x");
        printf(itoa((u8)&KernelEnd, Buffer, 16));
        printf("\n");

    } else {

        printf("\n[KERNEL] Multiboot header is NOT present!\n");
        return;

    }

    MultibootController Controller = MultibootController();

    Controller.Load(MultibootHeader);

    MMU MemoryController = MMU();

    printf("BUILD 8\n");

    MemoryController.Load(&Controller);

    World* RBuffer = (World*)MemoryController.Maps[3].Address+5*MB;

    Reflector = RBuffer;

    printf("[Reflector] free space - ");
    printf(itoa((MemoryController.Maps[3].Length-5*MB)/MB, Buffer, 10));
    printf("\n");

    Reflector->Initialize((MemoryController.Maps[3].Length-5*MB)/MB);

    Loader = Draco();

    printf("[Draco] Loading base classes\n");

    ClassFile CharacterClass = Loader.Load(Character_class);

    printf("[Draco] Loaded Character class\n");

    ClassFile DisplayClass = Loader.Load(Display_class);

    printf("[Draco] Loaded Display class\n");

    ClassFile ObjectClass = Loader.Load(Object_class);

    printf("[Draco] Loaded Object class\n");

    ClassFile PackedInterface = Loader.Load(Packed_class);

    printf("[Draco] Loaded Packed interface\n");

    ClassFile StringClass = Loader.Load(String_class);

    printf("[Draco] Loaded String class\n");

    ClassFile SystemClass = Loader.Load(System_class);

    printf("[Draco] Loaded System class\n");

    Reflector->AddClass(CharacterClass);

    Reflector->AddClass(DisplayClass);

    Reflector->AddClass(ObjectClass);

    Reflector->AddClass(PackedInterface);

    Reflector->AddClass(StringClass);

    Reflector->AddClass(SystemClass);

    printf("[Draco] Base classes loaded!\n");

    Stack InitialStack;
    Locals InitialRegisters;

    InitialStack.Initialize(10);
    InitialRegisters.Initialize(10);

    Class* EntryClass = Reflector->GetClass("java/lang/System");
    Method* EntryPoint = EntryClass->GetMethod("main","()V");

    printf("[Executive] Get control flow\n");
    printf("[Executive] Launching entry point\n");

    Executive::Execute(&EntryPoint->Attributes->Values[0].Code,0,&InitialStack,&InitialRegisters,EntryClass);

}
