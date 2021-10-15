#ifndef _VIC_II_HPP
#define _VIC_II_HPP

#include "Video.hpp"
#include "Memory.hpp"

#include "../aux/fifo.hpp"

#include <iostream>
#include <algorithm>

#include <cstring>
#include <cstdio>

typedef enum
{
  STANDARD_CHAR,
  MULTICOLOR_CHAR,
  STANDARD_BITMAP,
  MULTICOLOR_BITMAP,
  EXT_BG_COLOR_BITMAP
} t_video_mode;

class VICII : public Video
{
  protected:
    unsigned char *p;
    uint32_t*     screenPtr;
    uint32_t*     screenBase;
    uint32_t      pixelValue;
    int           screenCtr;
    int           memoryCtr;
    int           fastCtr;
    int           slowCtr;
    int           charROM_BaseAddr;
    int           screenRAM_BaseAddr;
    int           colorRAM_BaseAddr;
    uint8_t       memoryControlRegister; // 0xD018
    uint8_t       backgroundColor[4];
    t_video_mode  video_mode;
  public:
    VICII(fifo<unsigned char*>* videoStream, Memory* memory, int OSR);
    int runNextOperation(int CPU_CyclesPassed);
    uint8_t read  (uint16_t address);
    void    write (uint16_t address, uint8_t value);
};

#endif
