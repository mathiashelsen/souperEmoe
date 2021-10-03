#ifndef _DUMMY_MEMORY_HPP
#define _DUMMY_MEMORY_HPP

#include "Memory.hpp"

#include "../aux/c64_constants.hpp"

#include <fstream>
#include <iostream>

class DummyMemory : public Memory
{
  protected:
    const char* charROM_Name = "resources/c64.bin";
    uint8_t*    charROM;
    uint8_t*    videoRAM;
  public:
    DummyMemory(int nBytes);
    ~DummyMemory();
    uint8_t read(int addr);
};

#endif
