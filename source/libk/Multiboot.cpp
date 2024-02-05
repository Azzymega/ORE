#include "Multiboot.hpp"
#include "Display.hpp"

extern Display Console;

void MultibootController::Load(MultibootData *Object) {

  this->Data = Object;
  Console.Write("[KERNEL] Data is obtained\n");

}
