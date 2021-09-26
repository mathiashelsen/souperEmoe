#include "Video.hpp"

Video::Video(fifo<unsigned char *> *videoStream, Memory* memory)
{
  _memory       = memory;
  _videoStream  = videoStream;
}

int Video::runNextOperation(int CPU_CyclesPassed)
{
  colCtr += CPU_CyclesPassed;
  return 0;
}
