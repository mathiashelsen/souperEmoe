#include "MemoryController.hpp"

MemoryController::MemoryController(Video *video, Memory *memory)
{
  _video  = video;
  _memory = memory;

  LORAM   = false;      // Do not enable BASIC ROM
  HIRAM   = true;      // Do     enable KERNAL ROM
  CHAREN  = true;      // Do     enable the IO

}


uint8_t   MemoryController::read  (uint16_t address)
{
  if(address >= DEFAULT_KERNAL_ROM_BASE_ADDR)
  {
    if(HIRAM) {
      return _memory->read_kernal_rom(address - DEFAULT_KERNAL_ROM_BASE_ADDR);
    } else {
      return _memory->read           (address);
    }
  }

  /* The match should be on the entire IO range */
  if(address >= DEFAULT_START_IO_MEM_RANGE && address <= DEFAULT_END_IO_MEM_RANGE)
  {
    if(CHAREN) {
      if(address >= MEM_START_VICII_REG && address <= MEM_END_VICII_REG)
      {
        return _video->read(address);
      }
      if(address >= DEFAULT_COLOR_RAM_BASE_ADDR && address <= (DEFAULT_COLOR_RAM_BASE_ADDR+1023))
      {
        return _memory->read_color_ram(address - DEFAULT_COLOR_RAM_BASE_ADDR);
      }
    } else if(!HIRAM && !LORAM) {
      return _memory->read         (address);
    } else {
      return _memory->read_char_rom(address - DEFAULT_CHAR_ROM_BASE_ADDR);
    }
  }

  else
  {
    return _memory->read(address);
  }
}

void      MemoryController::write (uint16_t address, uint8_t data)
{
  if(address >= DEFAULT_KERNAL_ROM_BASE_ADDR)
  {
    if(!HIRAM) {
      _memory->write(address, data);
    }
  }
  else if(address >= DEFAULT_START_IO_MEM_RANGE && address <= DEFAULT_END_IO_MEM_RANGE)
  {
    if(CHAREN) {
      if(address >= MEM_START_VICII_REG && address <= MEM_END_VICII_REG)
      {
        return _video->write(address, data);
      }
      if(address >= DEFAULT_COLOR_RAM_BASE_ADDR && address <= (DEFAULT_COLOR_RAM_BASE_ADDR+1023))
      {
        return _memory->write_color_ram(address - DEFAULT_COLOR_RAM_BASE_ADDR, data);
      }
    } else if(!HIRAM && !LORAM) {
      _memory->write(address, data);
    }
  }
  else
  {
    _memory->write(address, data);
  }
}
