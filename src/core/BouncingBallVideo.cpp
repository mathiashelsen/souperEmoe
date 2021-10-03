#include "BouncingBallVideo.hpp"

BouncingBallVideo::BouncingBallVideo(fifo<unsigned char *>* videoStream, Memory *memory, int OSR) : Video(videoStream, memory, OSR)
{
  if(OSR != 1)
    std::cout << "This GPU does not support OSR != 1. Except the unexpected." << std::endl;

  ball_x  = rand() % SCREEN_XSIZE;
  ball_y  = rand() % SCREEN_YSIZE;
  dir_x   = 1;
  dir_y   = 1;

  rowCtr  = 0;
  colCtr  = 0;
}

int BouncingBallVideo::runNextOperation(int CPU_CyclesPassed)
{
  colCtr += CPU_CyclesPassed*8;
  if(colCtr >= VIDEO_TOTAL_WIDTH)
  {
    colCtr -= VIDEO_TOTAL_WIDTH;
    rowCtr += 1; // If the number of cycles that has passed, exceeds even an entire line, this is wrong
  }

  if( rowCtr >= VIDEO_TOTAL_HEIGHT )
  {
    rowCtr -= VIDEO_TOTAL_HEIGHT;

    ball_x += dir_x;
    ball_y += dir_y;

    if(ball_x >= SCREEN_XSIZE)
    {
      ball_x = SCREEN_XSIZE-1;
      dir_x  *= -1;
    }
    if(ball_y >= SCREEN_YSIZE)
    {
      ball_y = SCREEN_YSIZE-1;
      dir_y  *= -1;
    }

    if(ball_x < 1)
    {
      ball_x = 1;
      dir_x  *= -1;
    }

    if(ball_y < 1)
    {
      ball_y = 1;
      dir_y  *= -1;
    }

    unsigned char *p = (unsigned char*) malloc(SCREEN_XSIZE*SCREEN_YSIZE*4);
    memset((void *)p, 0, SCREEN_XSIZE*SCREEN_YSIZE*4);
   
    //int ballPixel = (ball_x + ball_y*SCREEN_XSIZE)*3;
    *((uint32_t *)p + ball_x + ball_y*SCREEN_XSIZE) = 0xffffff;
    *((uint32_t *)p + ball_x-1 + ball_y*SCREEN_XSIZE) = 0xffffff;
    *((uint32_t *)p + ball_x-2 + ball_y*SCREEN_XSIZE) = 0xffffff;
    *((uint32_t *)p + ball_x + (ball_y+1)*SCREEN_XSIZE) = 0xffffff;
    *((uint32_t *)p + ball_x + (ball_y-1)*SCREEN_XSIZE) = 0xffffff;

    _videoStream->push(p);
  }

  return 0;
  
}
