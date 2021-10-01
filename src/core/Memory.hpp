#ifndef _MEMORY_HPP
#define _MEMORY_HPP

#include "../aux/c64_constants.hpp"

#include <cstdlib>
#include <cstring>

#include <fstream>
#include <iostream>

class Memory
{
  protected:
    const char* charROM_Name = "resources/c64.bin";
    const char* executableName = "software/helloWorld.o";
    char* ram;
    int   ramSize; 
  public:
    Memory(int nBytes);
    Memory(int nBytes, const char *objectCodeFilename);
    ~Memory();
    virtual char read(int addr);
    virtual void write(int addr, char data);
};

#endif
