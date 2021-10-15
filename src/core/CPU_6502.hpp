#ifndef _CPU_6502_HPP
#define _CPU_6502_HPP

#include <iostream>
#include <cassert>

#include "CPU.hpp"
#include "ISA_6502.hpp"

typedef struct {
  unsigned int N : 1;
  unsigned int V : 1;
  unsigned int noConn : 1;
  unsigned int B : 1;
  unsigned int D : 1;
  unsigned int I : 1;
  unsigned int Z : 1;
  unsigned int C : 1;
} statusType;


class CPU_6502 : public CPU
{
  protected:
    char        acc;
    char        reg_x;
    char        reg_y;
    int         pc;
    uint8_t     sp;
    statusType  status;

    void        setStatusFlags(uint8_t val);
    uint8_t     getStatusFlags();
    void        updateFlagsNZ(char regVal);

  public:
    CPU_6502(MemoryController* memoryCtl);
    void        reset(void);
    int         runNextOperation(int IRQ, int NMI);
    int         runNextOperation() { return this->runNextOperation(0, 0); };
    char        getAcc(void)    { return acc; };
    char        getX(void)      { return reg_x; };
    char        getY(void)      { return reg_y; };
    int         getPC(void)     { return pc; };
    int         getSP(void)     { return sp; };
    statusType  getStatus(void) { return status; };

};

#endif
