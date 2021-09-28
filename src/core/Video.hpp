#ifndef _VIDEO_HPP
#define _VIDEO_HPP

#include "../aux/fifo.hpp"
#include "../aux/c64_constants.hpp"
#include "Memory.hpp"

class Video
{
  protected:
    fifo<unsigned char *>*  _videoStream;
    Memory*                 _memory;
    int                     rowCtr;
    int                     colCtr;
    int                     _OSR;
  public:
    Video(fifo<unsigned char *> *videoStream, Memory* memory, int OSR);
    virtual int runNextOperation(int CPU_CyclesPassed);
};

#endif
