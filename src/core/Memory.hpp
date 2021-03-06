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
    uint8_t *   charROM;
    uint8_t *   kernalROM;
    uint8_t *   colorRAM;
    int         ramSize;
    // Callback functions for setting/getting registers in other
    // chips (e.g. VIC-II)
    //void        (Video::*_setVICIIRegister)(uint16_t, uint8_t);
    //uint8_t     (Video::*_getVICIIRegister)(uint16_t);
  public:
    Memory(int nBytes);
    Memory(int nBytes, const char *objectCodeFilename);
    Memory(int nBytes, const char *objectCodeFilename, keystream* keyStream);
    ~Memory();
    virtual uint8_t read            (uint16_t addr);
    virtual uint8_t read_char_rom   (uint16_t addr);
    virtual uint8_t read_kernal_rom (uint16_t addr);
    virtual uint8_t read_color_ram  (uint16_t addr);
    virtual void write              (uint16_t addr, uint8_t data);
    virtual void write_color_ram    (uint16_t addr, uint8_t data);
};

#endif
