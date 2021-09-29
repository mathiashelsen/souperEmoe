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
    char* ram;
    int   ramSize; 
  public:
    Memory(int nBytes);
    ~Memory();
    virtual char read(int addr);
    virtual void write(int addr, char data);
};

#endif
