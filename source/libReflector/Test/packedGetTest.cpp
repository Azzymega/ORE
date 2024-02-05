#include <iostream>
#include "../Base.h"
#include "../Reflector.h"

void AllocTest(World* wd) {
    std::cout << "Allocation test! Iterations : " << 50000 << std::endl << std::endl;
    for (int i = 0; i < 50000; ++i) {
        long Value = rand();
        std::cout << "Value = " << Value << std::endl;
        GUID address = wd->Add(Value);
        std::cout << "GUID = " << address.Internal << std::endl << std::endl;
    }
    std::cout << "Time = " << (double)clock() << std::endl << std::endl;
    std::cout << "Test Passed!" << std::endl << std::endl;
}

void PackedAllocTest(World* wd) {
    std::cout << "Packed allocation test! Iterations : " << 5 << std::endl << std::endl;
    for (int i = 0; i < 5; ++i) {
        long Value = rand();
        std::cout << "Size = " << Value << std::endl;
        GUID address = wd->AddPacked(Value);
        std::cout << "GUID = " << address.Internal << std::endl << std::endl;
    }
    std::cout << "Time = " << (double)clock() << std::endl << std::endl;
    std::cout << "Test Passed!" << std::endl << std::endl;
}

struct x {
    int y;
    char str[];
};

void GetTest(World* wd) {
    for (int i = 0; i < 5; ++i) {
        long Value = rand();
        std::cout << "Value = " << Value << std::endl;
        GUID address = wd->Add(Value);
        std::cout << "GUID = " << address.Internal << std::endl;
        Object value = wd->Get(address);
        std::cout << "Find value = " << value << std::endl << std::endl;
    }
    std::cout << "Time = " << (double)clock() << std::endl << std::endl;
    std::cout << "Test Passed!" << std::endl << std::endl;
}

void PackedGetTest(World* wd) {
    GUID add = wd->AddPacked(14);
    std::cout << "GUID = " << add.Internal << std::endl;
    x* pp = static_cast<x *>(wd->GetPacked(add));
    pp->y = 13;
    memcpy(pp->str,"running",8);
    std::cout << "Y value = " << pp->y << std::endl;
    std::cout << "String value = " << pp->str << std::endl;
    std::cout << "Test Passed!" << std::endl << std::endl;
}

int main() {
    std::srand(std::time(nullptr));
    std::cout << "Initializing" << std::endl;
    auto* world = static_cast<World *>(malloc(MB * 10));
    world->Initialize(10);
    PackedGetTest(world);
    return 0;
}
