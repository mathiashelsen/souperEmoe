#ifndef _MEMORY_CONTROLLER_HPP
#define _MEMORY_CONTROLLER_HPP

#include <stdint.h>

#include "Video.hpp"
#include "Memory.hpp"

#include "../aux/c64_constants.hpp"

class MemoryController
{
  protected:
    Video*    _video;
    Memory*   _memory;
    bool      LORAM;
    bool      HIRAM;
    bool      CHAREN;
  public:
    MemoryController(Video* video, Memory* memory);
    uint8_t   read  (uint16_t address);
    void      write (uint16_t address, uint8_t data);

};

#endif
