#include "BouncingBallVideo.hpp"

BouncingBallVideo::BouncingBallVideo(fifo<unsigned char *>* videoStream, Memory *memory) : Video(videoStream, memory)
{
  ball_x  = rand() % XSIZE;
  ball_y  = rand() % YSIZE;
  dir_x   = 1;
  dir_y   = 1;

  rowCtr  = 0;
  colCtr  = 0;

  _videoStream = videoStream;
}

int BouncingBallVideo::runNextOperation(int CPU_CyclesPassed)
{
  colCtr += CPU_CyclesPassed*8;
  if(colCtr >= TOTAL_WIDTH)
  {
    colCtr -= TOTAL_WIDTH;
    rowCtr += 1; // If the number of cycles that has passed, exceeds even an entire line, this is wrong
  }

  if( rowCtr >= TOTAL_HEIGHT )
  {
    rowCtr -= TOTAL_HEIGHT;

    ball_x += dir_x;
    ball_y += dir_y;

    if(ball_x >= XSIZE)
    {
      ball_x = XSIZE-1;
      dir_x  *= -1;
    }
    if(ball_y >= YSIZE)
    {
      ball_y = YSIZE-1;
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

    unsigned char *p = (unsigned char*) malloc(XSIZE*YSIZE*4);
    memset((void *)p, 0, XSIZE*YSIZE*4);
   
    int ballPixel = (ball_x + ball_y*XSIZE)*3;
    *((uint32_t *)p + ball_x + ball_y*XSIZE) = 0xffffff;
    *((uint32_t *)p + ball_x-1 + ball_y*XSIZE) = 0xffffff;
    *((uint32_t *)p + ball_x-2 + ball_y*XSIZE) = 0xffffff;
    *((uint32_t *)p + ball_x + (ball_y+1)*XSIZE) = 0xffffff;
    *((uint32_t *)p + ball_x + (ball_y-1)*XSIZE) = 0xffffff;

    _videoStream->push(p);
  }

  return 0;
  
}
