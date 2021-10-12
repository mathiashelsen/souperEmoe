#ifndef _VIC_II_HPP
#define _VIC_II_HPP

#include "Video.hpp"
#include "Memory.hpp"

#include "../aux/fifo.hpp"

#include <iostream>
#include <algorithm>

#include <cstring>
#include <cstdio>

class VICII : public Video
{
  protected:
    unsigned char *p;
    uint32_t*     screenPtr;
    uint32_t*     screenBase;
    uint32_t      frontColor;
    uint32_t      backColor;
    uint32_t      pixelValue;
    int           screenCtr;
    int           memoryCtr;
    int           fastCtr;
    int           slowCtr;
    int           charROM_BaseAddr;
    int           screenRAM_BaseAddr;
    uint8_t       memoryControlRegister; // 0xD018
  public:
    VICII(fifo<unsigned char*>* videoStream, Memory* memory, int OSR);
    int runNextOperation(int CPU_CyclesPassed);
    uint8_t getRegister(uint16_t address);
    void    setRegister(uint16_t address, uint8_t value);
};

#endif
