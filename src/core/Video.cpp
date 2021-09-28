#include "Video.hpp"

Video::Video(fifo<unsigned char *> *videoStream, Memory* memory, int OSR)
{
  _memory       = memory;
  _videoStream  = videoStream;
  _OSR          = OSR;
}

int Video::runNextOperation(int CPU_CyclesPassed)
{
  colCtr += CPU_CyclesPassed;
  return 0;
}
