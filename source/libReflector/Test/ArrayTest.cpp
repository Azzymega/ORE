#include <stdlib.h>
#include <ctime>
#include <iostream>
#include "../Reflector.h"

void ArrayTest(World* wd) {
    GUID Data = wd->AddArray(10);
    GUID value = wd->Add((long)0xcafebabe);
    GUID svalue = wd->Add(0xddd);
    std::cout << "Added value: " << wd->Get(value) << std::endl;
    std::cout << "Added value: " << wd->Get(svalue) << std::endl;
    wd->SetArrayValue(Data,0,value);
    wd->SetArrayValue(Data,1,svalue);
    GUID GetValue = wd->GetArrayValue(Data,0);
    GUID SGetValue = wd->GetArrayValue(Data,1);
    std::cout << "Value: " << wd->Get(GetValue) << std::endl;
    std::cout << "Value: " << wd->Get(SGetValue) << std::endl;
    std::cout <<  "Array test passed!" << std::endl;
}

int main() {
    std::srand(std::time(nullptr));
    std::cout << "Initializing Array Test!!!" << std::endl;
    auto* world = static_cast<World *>(malloc(MB * 10));
    world->Initialize(10);
    ArrayTest(world);
    return 0;
}
