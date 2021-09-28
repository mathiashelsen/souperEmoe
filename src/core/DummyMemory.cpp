#include "DummyMemory.hpp"

DummyMemory::DummyMemory(int nBytes) : Memory(nBytes)
{
  charROM   = (char*) malloc(4096);
  videoRAM  = (char*) malloc(2024);
  std::ifstream fileStream;

  fileStream.open(charROM_Name, std::ifstream::in);
  fileStream.get(charROM, 4096);
  fileStream.close();

  for(int i = 0; i < 2024; i++)
  {
    videoRAM[i] = i % 127;
  }
}

DummyMemory::~DummyMemory()
{
  free(charROM);
  free(videoRAM);
}

char DummyMemory::read(int addr)
{
  // This is pure horror, yes I know. It's only for debug
  if(addr >= DEFAULT_CHAR_ROM_BASE_ADDR)
    return charROM[addr - DEFAULT_CHAR_ROM_BASE_ADDR];

  if(addr >= DEFAULT_SCREEN_RAM_BASE_ADDR)
    return videoRAM[addr-DEFAULT_SCREEN_RAM_BASE_ADDR];

  return 0;
}
