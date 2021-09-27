#include "VICII.hpp"

static uint32_t colormap[] =
  {
    0x000000,
    0xffffff,
    0x880000,
    0xaaffee,
    0xcc44cc,
    0x00cc55,
    0x0000aa,
    0xeeee77,
    0xdd8855,
    0x664400,
    0xff7777,
    0x333333,
    0x777777,
    0xaaff66,
    0x0088ff,
    0xbbbbbb
  };

VICII::VICII(fifo<unsigned char*>* videoStream, Memory* memory) : Video(videoStream, memory)
{
  colCtr = 0;
  rowCtr = 0;

  p = (unsigned char*) malloc(SCREEN_XSIZE*SCREEN_YSIZE*4);
  memset((void *)p, 0, SCREEN_XSIZE*SCREEN_YSIZE*4);
  pixelPtr = (uint32_t *) p;
}

int VICII::runNextOperation(int CPU_CyclesPassed)
{
  colCtr += CPU_CyclesPassed*8;

  if(colCtr >= VIDEO_TOTAL_WIDTH)
  {
    colCtr -= VIDEO_TOTAL_WIDTH;
    rowCtr += 1; // If the number of cycles that has passed, exceeds even an entire line, this is wrong
  }

  // Check if we are in the active part of the screen. For simplicity, the entire active part
  // of the screen is put at the very last part. This corresponds with an arbitrary phase shift
  if(colCtr > COL_DEADTIME && rowCtr > ROW_DEADTIME)
  {
    int cellsToCopy = std::min(colCtr - COL_DEADTIME, CPU_CyclesPassed)*8;
    //std::cout << "Going to copy " << cellsToCopy << " of pixels" << std::endl;
    for(int i = 0; i < cellsToCopy; i++)
    {
      *pixelPtr++ = colormap[14]; // Paint all pixels light blue
    }
  }

  if( rowCtr >= VIDEO_TOTAL_HEIGHT )
  {
    rowCtr -= VIDEO_TOTAL_HEIGHT;

    printf("First value of pointer %p = 0x%x\n", p, p[0]);
    _videoStream->push(p);

    p = (unsigned char*) malloc(SCREEN_XSIZE*SCREEN_YSIZE*4);
    memset((void *)p, 0, SCREEN_XSIZE*SCREEN_YSIZE*4);
    pixelPtr = (uint32_t *) p;
  }

  return 0;
}
