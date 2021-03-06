#include "VICII.hpp"

static uint32_t colormap[] =
  {
    0x000000,
    0xffffff,
    0x68372b,
    0x70a4b2,
    0x6f3d86,
    0x588d43,
    0x352879,
    0xb8c76f,
    0x6f4f25,
    0x433900,
    0x9a6759,
    0x444444,
    0x6c6c6c,
    0x9ad284,
    0x6c5eb5,
    0x959595
  };

VICII::VICII(fifo<unsigned char*>* videoStream, Memory* memory, int OSR) : Video(videoStream, memory, OSR)
{
  colCtr                = 0;
  rowCtr                = 0;

  p                     = (unsigned char*) malloc(SCREEN_XSIZE*SCREEN_YSIZE*4*_OSR*_OSR);
  memset((void *)p, 0, SCREEN_XSIZE*SCREEN_YSIZE*4*_OSR*_OSR);
  screenPtr             = (uint32_t *) p;
  screenBase            = screenPtr;
  screenCtr             = 0;
  memoryCtr             = 0;
  fastCtr               = 0;
  slowCtr               = 0;

  memoryBankCfg         = 3;
  charROM_BaseAddr      = DEFAULT_CHAR_ROM_BASE_ADDR;
  screenRAM_BaseAddr    = DEFAULT_SCREEN_RAM_BASE_ADDR;
  colorRAM_BaseAddr     = DEFAULT_COLOR_RAM_BASE_ADDR;

  memoryControlRegister = 0b00010010;

  video_mode            = STANDARD_CHAR;
}

int VICII::runNextOperation(int CPU_CyclesPassed)
{
  uint8_t       tmp;
  uint32_t      frontColor;
  uint32_t      backColor;
  int cellsToCopy = 0;
  uint8_t memcfg = (memoryControlRegister >> 4) & 0x0F;
  screenRAM_BaseAddr  = (3 - memoryBankCfg)*0x4000 + memcfg * DEFAULT_SCREEN_RAM_BASE_ADDR;
  memcfg = (memoryControlRegister >> 1) & 0b111;
  charROM_BaseAddr    = memcfg * DEFAULT_CHAR_ROM_BASE_ADDR;
  if(memoryBankCfg == 0 || memoryBankCfg == 2)
  {
    charROM_BaseAddr  += (3 - memoryBankCfg)*0x4000;
  }

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

  char lineOfChar = (rowCtr-1) % 8;

  /*
   * COPY REQUIRED PIXELS (IF ANY)
   */
  if( cellsToCopy % 8 != 0)
    throw "Error: pixels to copy is not a multiple of 8";

  for(int i = 0; i < cellsToCopy/8; i++)
  {
    char charToShow = _memory->read           (screenRAM_BaseAddr + fastCtr + slowCtr*40);
    char charColor  = _memory->read_color_ram (                     fastCtr + slowCtr*40);
    char readByte   = 0;
    if(memoryBankCfg == 1 || memoryBankCfg == 3)
    {
      readByte      = _memory->read_char_rom(charROM_BaseAddr   + charToShow*8 + lineOfChar);
    }else{
      readByte      = _memory->read         (charROM_BaseAddr   + charToShow*8 + lineOfChar);
    }

    //char readByte = 0x0F; // You actually want to read from memory here
    fastCtr++;
    // We have transferred a row
    if(fastCtr == VIDEO_XSIZE/8)
    {
      fastCtr = 0;
      if(rowCtr % 8 == 7)
        slowCtr++;

    }

    for(int i = 0; i < 8; i++)
    {
      if(video_mode == STANDARD_CHAR)
      {
        frontColor = colormap[charColor & 0x0f];
        backColor  = colormap[backgroundColor[0]];
        pixelValue = (((readByte & 0x80) >> 7) * frontColor) | (((~readByte & 0x80) >> 7) * backColor);
        readByte <<= 1;
      }
      else if(video_mode == MULTICOLOR_CHAR)
      {
        tmp = (readByte >> 6) & 0x03;
        if(tmp == 0x03)
        {
          pixelValue = colormap[charColor & 0x0f];
        }
        else
        {
          pixelValue = colormap[backgroundColor[tmp]];
        }

        if( i % 2 == 1)
        {
          readByte <<= 2;
        }
      }
      else
      {
        pixelValue = 0;
      }
       
      switch(_OSR)
      {
        case 1:
          *screenPtr++ = pixelValue;
          screenCtr++;
          break;
        case 2:
        {
          *screenPtr                       = pixelValue;
          *(screenPtr+1)                   = pixelValue;
          *(screenPtr+_OSR*SCREEN_XSIZE)   = pixelValue;
          *(screenPtr+_OSR*SCREEN_XSIZE+1) = pixelValue;

          screenPtr += _OSR;
          screenCtr += _OSR*_OSR;
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
          *(screenPtr                       ) = pixelValue;
          *(screenPtr+1                     ) = pixelValue;
          *(screenPtr+2                     ) = pixelValue;
          *(screenPtr+_OSR*SCREEN_XSIZE     ) = pixelValue;
          *(screenPtr+_OSR*SCREEN_XSIZE+1   ) = pixelValue;
          *(screenPtr+_OSR*SCREEN_XSIZE+2   ) = pixelValue;
          *(screenPtr+2*_OSR*SCREEN_XSIZE   ) = pixelValue;
          *(screenPtr+2*_OSR*SCREEN_XSIZE+1 ) = pixelValue;
          *(screenPtr+2*_OSR*SCREEN_XSIZE+2 ) = pixelValue;

          screenPtr += _OSR;
          screenCtr += _OSR*_OSR;
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
    memoryCtr   = 0;
    fastCtr     = 0;
    slowCtr     = 0;
  }

  return 0;
}


uint8_t VICII::read(uint16_t address)
{
  if(address == 0xD018)
  {
    return memoryControlRegister;
  }
  if( address >= 0xD021 && address <= 0xD024 )
  {
    return backgroundColor[address - 0xD021];
  }
  if( address == 0xDD00 )
  {
    return memoryBankCfg;
  }


  return 0;
}
void    VICII::write(uint16_t address, uint8_t value)
{
  if(address == 0xD018)
  {
    memoryControlRegister = value;
  }
  if(address >= 0xD021 && address <= 0xD024)
  {
    backgroundColor[address - 0xD021] = value;
  }
  if(address == 0xDD00)
  {
    memoryBankCfg = value & 0b11;
  }
  if(address == 0xD016)
  {
    if(value & 0x10)
    {
      video_mode = MULTICOLOR_CHAR; 
    }
    else
    {
      video_mode = STANDARD_CHAR;
    }
  }
}
