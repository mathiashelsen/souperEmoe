#include "CPU.hpp"

CPU::CPU(Memory *memory)
{
  _memory = memory;
}

int CPU::runNextOperation(void)
{
  pc++;
  //return 1;
  return (rand() % 7) + 1;
}

int CPU::runNextOperation(int IRQ, int NMI)
{
  pc++;
  //return 1;
  return (rand() % 7) + 1;
}

void CPU::reset(void)
{
  pc = 0;
}
