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
    virtual void reset(void);
    virtual int runNextOperation(void);
    virtual int runNextOperation(int, int);
};

#endif
