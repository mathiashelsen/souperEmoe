#include "Memory.hpp"

Memory::Memory(int nBytes)
{
  ram     = (char *)malloc(nBytes);
  ramSize = nBytes;

  memset(ram, 0, nBytes);

  std::ifstream fileStream;

  fileStream.open(charROM_Name, std::ifstream::in);
  fileStream.get(ram + DEFAULT_CHAR_ROM_BASE_ADDR, 4096);
  fileStream.close();
  for(int i = 0; i < 2024; i++)
  {
    ram[i+DEFAULT_SCREEN_RAM_BASE_ADDR] = 32; //Empty char
  }

  fileStream.open(executableName, std::ifstream::in);
  fileStream.get(ram, 1024);
  fileStream.close();

}

Memory::Memory(int nBytes, const char* objectCodeFilename)
{
  ram     = (char *)malloc(nBytes);
  ramSize = nBytes;

  memset(ram, 0, nBytes);

  std::ifstream fileStream;

  fileStream.open(charROM_Name, std::ifstream::in);
  fileStream.get(ram + DEFAULT_CHAR_ROM_BASE_ADDR, 4096);
  fileStream.close();
  for(int i = 0; i < 2024; i++)
  {
    ram[i+DEFAULT_SCREEN_RAM_BASE_ADDR] = 32; //Empty char
  }

  fileStream.open(objectCodeFilename, std::ifstream::in);
  fileStream.get(ram, 1024);
  fileStream.close();

}

Memory::~Memory()
{
  free(ram);
}

char Memory::read(int addr)
{
  printf("Reading value 0x%02X from 0x%04X\n", (uint8_t) ram[addr], (uint16_t) addr);
  if(addr < ramSize)
    return ram[addr];
  else
    return (char) 0;
}

void Memory::write(int addr, char data)
{
  printf("Writing value 0x%02X to address %04X\n", (uint8_t) data, (uint16_t) addr);
  if(addr < ramSize)
    ram[addr] = data;
}
