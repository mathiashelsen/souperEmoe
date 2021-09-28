#include "DummyMemory.hpp"

DummyMemory::DummyMemory(int nBytes) : Memory(nBytes)
{
  charROM = (char*) malloc(256);
  std::ifstream fileStream;

  fileStream.open(charROM_Name, std::ifstream::in);

  fileStream.get(charROM, 256);

  fileStream.close();
}

DummyMemory::~DummyMemory()
{
  free(charROM);
}

char DummyMemory::read(int addr)
{
  if(addr >= DEFAULT_CHAR_ROM_BASE_ADDR)
    return charROM[addr - DEFAULT_CHAR_ROM_BASE_ADDR];

  return 0;
}
