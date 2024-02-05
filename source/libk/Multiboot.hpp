#pragma once
#include "stdlib.hpp"
#include "Display.hpp"


struct BootDevices {
  u1 FourDevice;
  u1 ThreeDevice;
  u1 SecondDevice;
  u1 MainDrive;
};

struct NativeMemoryData {
  u4 LowerMemory;
  u4 UpperMemory;
};

struct MultibootModules {
  u4 ModulesCount;
  u4 ModulesAddress;
};

enum DriveMode : u1 { CHS, LBA };

struct MemoryMapInfo {
  u4 MemoryMapLength;
  u4 MemoryMapAddress;
};

struct Drives {
  u4 DrivesLength;
  u4 DrivesAddress;
};

struct Drive {
  u4 Size;
  u1 DriveNumber;
  enum DriveMode DriveMode;
  u2 DriveCylinders;
  u2 DriveHeads;
  u1 DriveSectors;
  // u2 DrivePorts;
};

struct KernelELFData {
  u4 Number;
  u4 Size;
  u4 Address;
  u4 SHNDX;
};

struct CommandLine {
  char CMD[4];
};

struct ConfigTable {
  u4 ConfigTable;
};

enum MemoryMapType : u4 {
  Avaliable = 1,
  Reserved = 2,
  ACPI_Reclaimable = 3,
  NVS = 4,
  BADRAM = 5
};

struct MultibootData {
  u4 Flags;
  struct NativeMemoryData Memory;
  struct BootDevices BootDevices;
  struct CommandLine CMDLine;
  struct MultibootModules Modules;
  struct KernelELFData ELFData;
  struct MemoryMapInfo MemoryMapData;
  struct Drives DrivesData;
  struct ConfigTable CFGTable;
  // Implement others
};

struct MultibootController {
  MultibootData *Data;
  void Load(MultibootData *Object);
};