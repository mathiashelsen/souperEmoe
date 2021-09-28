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
    char*       charROM;
    char*       videoRAM;
  public:
    DummyMemory(int nBytes);
    ~DummyMemory();
    char read(int addr);
};

#endif
