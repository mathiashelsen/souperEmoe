#ifndef _BOUNCING_BALL_VIDEO
#define _BOUNCING_BALL_VIDEO

#include "Video.hpp"
#include "Memory.hpp"

#include "../aux/c64_constants.hpp"

#include <cstdlib>
#include <cstring>

#include <iostream>

class BouncingBallVideo : public Video
{
  protected:
    int ball_x;
    int ball_y;
    int dir_x;
    int dir_y;
  public:
    BouncingBallVideo(fifo<unsigned char*>* videoStream, Memory* memory);
    int runNextOperation(int CPU_CyclesPassed);
};

#endif
