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

VICII::VICII(fifo<unsigned char*>* videoStream, Memory* memory, int OSR) : Video(videoStream, memory, OSR)
{
  colCtr = 0;
  rowCtr = 0;

  p           = (unsigned char*) malloc(SCREEN_XSIZE*SCREEN_YSIZE*4*_OSR*_OSR);
  memset((void *)p, 0, SCREEN_XSIZE*SCREEN_YSIZE*4*_OSR*_OSR);
  screenPtr   = (uint32_t *) p;
  screenBase  = screenPtr;
  screenCtr   = 0;
  memoryCtr   = 0;
}

int VICII::runNextOperation(int CPU_CyclesPassed)
{
  int cellsToCopy = 0;
  if(rowCtr > ROW_DEADTIME)
  {
    // Check if we have to discard a few cycles because they were invalid
    if(colCtr < COL_DEADTIME)
      cellsToCopy = std::max(0, (colCtr - COL_DEADTIME + CPU_CyclesPassed*8));
    else
      cellsToCopy = CPU_CyclesPassed*8;

    if(VIDEO_TOTAL_WIDTH-colCtr < cellsToCopy)
      cellsToCopy = VIDEO_TOTAL_WIDTH-colCtr;
  }

  //std::cout << "Have to copy " << cellsToCopy << " pixels, already did " << screenCtr << ", " << colCtr << ", " << rowCtr << std::endl;

  /*
   * COPY REQUIRED PIXELS (IF ANY)
   */
  for(int i = 0; i < cellsToCopy; i++)
  {
    switch(_OSR)
    {
      case 1:
        *screenPtr++ = colormap[14]; // Paint all pixels light blue
        screenCtr++;
        memoryCtr++;
        break;
      case 2:
      {
        *screenPtr                       = colormap[14];
        //*(screenPtr+1)                   = colormap[14];
        //*(screenPtr+_OSR*SCREEN_XSIZE)   = colormap[14];
        *(screenPtr+_OSR*SCREEN_XSIZE+1) = colormap[13];

        screenPtr += _OSR;
        screenCtr += _OSR*_OSR;
        memoryCtr++;
        int a = (int) (screenPtr - screenBase);
        int b = (int) _OSR*SCREEN_XSIZE;
        if( (a % b) == 0 )
        {
          screenPtr += _OSR*SCREEN_XSIZE;
        }

        break;
      }
      case 3:
      {
        *(screenPtr                       ) = colormap[14];
        *(screenPtr+1                     ) = colormap[14];
        *(screenPtr+2                     ) = colormap[14];
        *(screenPtr+_OSR*SCREEN_XSIZE     ) = colormap[14];
        *(screenPtr+_OSR*SCREEN_XSIZE+1   ) = colormap[14];
        *(screenPtr+_OSR*SCREEN_XSIZE+2   ) = colormap[14];
        *(screenPtr+2*_OSR*SCREEN_XSIZE   ) = colormap[14];
        *(screenPtr+2*_OSR*SCREEN_XSIZE+1 ) = colormap[14];
        *(screenPtr+2*_OSR*SCREEN_XSIZE+2 ) = colormap[14];

        screenPtr += _OSR;
        screenCtr += _OSR*_OSR;
        memoryCtr++;
        int a = (int) (screenPtr - screenBase);
        int b = (int) _OSR*SCREEN_XSIZE;
        if( (a % b) == 0 )
        {
          screenPtr += (_OSR-1)*_OSR*SCREEN_XSIZE;
        }
        break;
      }
  }
}

  /*
   * INCREMENT/WRAP COUNTERS FOR VSYNC/HSYNC
   */
  colCtr += CPU_CyclesPassed*8;
  if(colCtr >= VIDEO_TOTAL_WIDTH)
  {
    colCtr -= VIDEO_TOTAL_WIDTH;
    rowCtr += 1; // If the number of cycles that has passed, exceeds even an entire line, this is wrong
  }

  if( rowCtr > VIDEO_TOTAL_HEIGHT )
  {
    rowCtr -= VIDEO_TOTAL_HEIGHT;

    _videoStream->push(p);

    p           = (unsigned char*) malloc(SCREEN_XSIZE*SCREEN_YSIZE*4*_OSR*_OSR);
    memset((void *)p, 0, SCREEN_XSIZE*SCREEN_YSIZE*4*_OSR*_OSR);
    screenPtr   = (uint32_t *) p;
    screenBase  = screenPtr;
    screenCtr   = 0;
    memoryCtr   = 0;
  }

  return 0;
}
