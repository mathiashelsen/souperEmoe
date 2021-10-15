#include "MemoryController.hpp"

MemoryController::MemoryController(Video *video, Memory *memory)
{
  _video  = video;
  _memory = memory;
}


uint8_t   MemoryController::read  (uint16_t address)
{
  /*
   * Here we need to have some dependency on the memory configuration
   */
   printf("Read request for address: 0x%04X\n", address);

  if(address >= MEM_START_VICII_REG && address <= MEM_END_VICII_REG)
  {
    return _video->read(address);
  }
  if(address >= DEFAULT_KERNAL_ROM_BASE_ADDR && address < NMI_VECTOR)
  {
    return _memory->read_kernal_rom(address - DEFAULT_KERNAL_ROM_BASE_ADDR);
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

  if(address >= MEM_START_VICII_REG && address <= MEM_END_VICII_REG)
  {
    _video->write(address, data);
  }
  else
  {
    _memory->write(address, data);
  }
}
