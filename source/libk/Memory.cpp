#include "Memory.hpp"

extern Display Console;

void MMU::Load(MultibootController *Object) {
  u4 y = 0;
  for (u4 x = 0; x < Object->Data->MemoryMapData.MemoryMapLength;
       x += sizeof(MemoryMap)) {

    char Address[16];
    char Length[16];

    this->Maps[y] =
        *(MemoryMap *)(Object->Data->MemoryMapData.MemoryMapAddress + x);

    Console.Write("[MMU] Memory map start: 0x");
    Console.Write(itoa(this->Maps[y].Address, Address, 16));
    Console.Write(", length: 0x");
    Console.Write(itoa(this->Maps[y].Length, Length, 16));
    Console.Write(", type: ");

    switch (this->Maps[y].Type) {
      
      case Avaliable:
        if (this->Maps[y].Address == 0) {
          this->Maps[y].Type = Reserved;
          Console.Write("Reserved\n");
          break;
        }
        Console.Write("Available\n");
        break;
      case Reserved:
        Console.Write("Reserved\n");
        break;
      case ACPI_Reclaimable:
        Console.Write("ACPI\n");
        break;
      case NVS:
        Console.Write("NVS\n");
        break;
      case BADRAM:
        Console.Write("BADRAM\n");
        break;
      }

    y++;

  }
}

Chunck::Chunck() {
  this->ElementsCount = 0;
  this->Size = 0;
  this->StartAddress = nullptr;
  this->SizeAvailable = 0;
}

void Heap::Load(MMU *Object) {
  for (auto &&chunk : Object->Maps) {
    if (chunk.Type == Avaliable) {
      this->Main.SizeAvailable = chunk.Length;
      this->Main.StartAddress = (char *)(chunk.Address + 1000000);
      this->Main.CurrentAddress = this->Main.StartAddress;
      Console.Write("[MMU] Chunck founded!\n");
      break;
    }
  }
}
