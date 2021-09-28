#ifndef _MEMORY_HPP
#define _MEMORY_HPP

#include <cstdlib>

class Memory
{
  protected:
    char* ram;
    int   ramSize; 
  public:
    Memory(int nBytes);
    ~Memory();
    virtual char read(int addr);
    virtual void write(int addr, char data);
};

#endif
