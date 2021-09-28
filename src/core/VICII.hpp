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
    int           charROM_BaseAddr;
  public:
    VICII(fifo<unsigned char*>* videoStream, Memory* memory, int OSR);
    int runNextOperation(int CPU_CyclesPassed);
};

#endif
