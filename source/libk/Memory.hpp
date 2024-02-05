#pragma once
#include "stdlib.hpp"
#include "../libc/stdlib.hpp"
#include "Multiboot.hpp"
#include "Display.hpp"


struct MemoryMap {
  u4 Size;
  u8 Address;
  u8 Length;
  MemoryMapType Type;
} __attribute__((packed));

struct Chunck {
  Chunck();
  char *StartAddress;
  char *CurrentAddress;
  u4 Size;
  u4 SizeAvailable;
  u4 ElementsCount;
};

struct MMU;

struct Heap {
  Chunck Main;
  u8 PointerCount;
  void Load(MMU *Object);
};

struct MMU {
  MemoryMap Maps[15];
  void Load(MultibootController *Object);
};
