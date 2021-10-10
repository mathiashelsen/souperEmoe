#ifndef _MEMORY_HPP
#define _MEMORY_HPP

#include "../aux/c64_constants.hpp"
#include "../aux/keystream.hpp"

#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>

class Memory
{
  protected:
    const char* charROM_Name = "resources/c64.bin";
    const char* executableName = "software/helloWorld.o";
    keystream*  _keyStream;
    uint8_t * ram;
    int   ramSize; 
  public:
    Memory(int nBytes);
    Memory(int nBytes, const char *objectCodeFilename);
    Memory(int nBytes, const char *objectCodeFilename, keystream* keyStream);
    ~Memory();
    virtual uint8_t read(int addr);
    virtual void write(int addr, uint8_t data);
};

#endif
