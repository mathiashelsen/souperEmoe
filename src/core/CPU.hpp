#ifndef _CPU_HPP
#define _CPU_HPP

#include "Memory.hpp"

class CPU
{
  protected:
    Memory* _memory;
    int pc;
  public:
    CPU(Memory* memory);
    virtual int runNextOperation(void);
};

#endif
