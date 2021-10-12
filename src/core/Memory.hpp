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
    uint8_t *   ram;
    uint8_t *   VICII_Register;
    int         ramSize;
    // Callback functions for setting/getting registers in other
    // chips (e.g. VIC-II)
    //void        (Video::*_setVICIIRegister)(uint16_t, uint8_t);
    //uint8_t     (Video::*_getVICIIRegister)(uint16_t);
  public:
    Memory(int nBytes);
    Memory(int nBytes, const char *objectCodeFilename);
    Memory(int nBytes, const char *objectCodeFilename, keystream* keyStream);
    //void setVICII_Callbacks(void (Video::*setVICIIRegister)(uint16_t, uint8_t),uint8_t (Video::*getVICIIRegister)(uint16_t));
    ~Memory();
    virtual uint8_t read(int addr);
    virtual void write(int addr, uint8_t data);
};

#endif
