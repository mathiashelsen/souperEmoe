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
    int           screenCtr;
    int           memoryCtr;
  public:
    VICII(fifo<unsigned char*>* videoStream, Memory* memory, int OSR);
    int runNextOperation(int CPU_CyclesPassed);
};

#endif
