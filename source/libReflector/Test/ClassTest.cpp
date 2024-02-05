#include <stdlib.h>
#include <ctime>
#include <iostream>
#include "../Reflector.h"
#include "../Extension/Class.h"

World* Reflector;

void ClassTest(World* wd) {
    int read;
    int counter = 0;
    FILE *fp = fopen("String.Class", "rb");
    fseek(fp, 0, SEEK_END);
    u1* data = (u1 *) malloc(ftell(fp));
    fseek(fp, 0, SEEK_SET);
    while ((read = getc(fp)) != EOF) {
        data[counter] = read;
        counter++;
    }
    Draco dr = Draco();
    ClassFile File = dr.Load(data);
    GUID JavaLangStringClass = wd->AddClass(File);
    Class* Pointer = wd->GetClass(JavaLangStringClass);
    assert(Pointer == nullptr, "Test failed!!!");
    assert(Pointer->File.Magic != 0xcafebabe, "Test failed!!!");
    Class* PointerString = wd->GetClass("java/lang/String");
    std::cout << "Test passed!!!" << std::endl;
}

int main() {
    std::srand(std::time(nullptr));
    std::cout << "Initializing Class Test!!!" << std::endl;
    Reflector = static_cast<World *>(malloc(MB * 10));
    Reflector->Initialize(10);
    ClassTest(Reflector);
    return 0;
}
