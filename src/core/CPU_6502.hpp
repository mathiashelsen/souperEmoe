#ifndef _CPU_6502_HPP
#define _CPU_6502_HPP

#include <iostream>

#include "CPU.hpp"

class CPU_6502 : public CPU
{
  protected:
    int   acc;
    int   reg_x;
    int   reg_y;
    int   pc;
    int   sp;
    struct {
      unsigned int N : 1;
      unsigned int V : 1;
      unsigned int noConn : 1;
      unsigned int B : 1;
      unsigned int D : 1;
      unsigned int I : 1;
      unsigned int Z : 1;
      unsigned int C : 1;
    } status;

  public:
    CPU_6502(Memory* memory);
    int runNextOperation(void);

};

#endif
