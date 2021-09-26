#include "Memory.hpp"

Memory::Memory(int nBytes)
{
  ram     = (char *)malloc(nBytes);
  ramSize = nBytes;
}

Memory::~Memory()
{
  free(ram);
}

char Memory::read(int addr)
{
  if(addr < ramSize)
    return ram[addr];
  else
    return (char) 0;
}

void Memory::write(int addr, char data)
{
  if(addr < ramSize)
    ram[addr] = data;
}
