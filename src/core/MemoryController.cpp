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
  if(address >= MEM_START_VICII_REG && address <= MEM_END_VICII_REG)
  {
    if(CHAREN) {
      return _video->read(address);
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
  /*
   * Here we need to have some dependency on the memory configuration
   */
  if(address >= DEFAULT_KERNAL_ROM_BASE_ADDR)
  {
    if(!HIRAM) {
      _memory->write(address, data);
    }
  }
  else if(address >= MEM_START_VICII_REG && address <= MEM_END_VICII_REG)
  {
    if(CHAREN) {
      _video-> write(address, data);
    } else if(!HIRAM && !LORAM) {
      _memory->write(address, data);
    }
  }
  else
  {
    _memory->write(address, data);
  }
}
