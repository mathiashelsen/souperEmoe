#ifndef _CPU_HPP
#define _CPU_HPP

#include "MemoryController.hpp"

class CPU
{
  protected:
    MemoryController* _memoryCtl;
    int pc;
  public:
    CPU(MemoryController* memoryCtl);
    virtual void reset(void);
    virtual int runNextOperation(void);
    virtual int runNextOperation(int, int);
};

#endif
