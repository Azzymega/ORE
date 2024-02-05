#include <cstdio>
#include <cstdlib>
#include "../Loader.h"
#include "../../Reflector/Reflector.h"

World* Reflector;

int main() {
    World* wd = static_cast<World *>(malloc(MB * 10));
    Reflector = wd;
    Reflector->Initialize(10);
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
    return 0;
}