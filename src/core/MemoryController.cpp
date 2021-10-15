#include "MemoryController.hpp"

MemoryController::MemoryController(Video *video, Memory *memory)
{
  _video  = video;
  _memory = memory;
}


uint8_t   MemoryController::read  (uint16_t address)
{
  if(address >= MEM_START_VICII_REG && address <= MEM_END_VICII_REG)
  {
    return _video->read(address);
  }
  else
  {
    return _memory->read(address);
  }
}

void      MemoryController::write (uint16_t address, uint8_t data)
{
  if(address >= MEM_START_VICII_REG && address <= MEM_END_VICII_REG)
  {
    _video->write(address, data);
  }
  else
  {
    _memory->write(address, data);
  }

  if(address >= 0xd800 && address <= 0xdbe8)
  {
    printf("Wrote 0x%02X to 0x%04X\n", data, address);
  }

}
