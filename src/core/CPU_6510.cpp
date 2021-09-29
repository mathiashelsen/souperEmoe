#include "CPU_6510.hpp"

CPU_6510::CPU_6510(Memory *  memory) : CPU(memory)
{
  acc     = 0;
  reg_x   = 0;
  reg_y   = 0;
  pc      = 0;
  sp      = DEFAULT_STACK_BASE_ADDR;

  status.N = 0;
  status.V = 0;
  status.B = 0;
  status.D = 0;
  status.I = 0;
  status.Z = 0;
  status.C = 0;
}

int CPU_6510::runNextOperation()
{
  uint8_t instruction = _memory->read(pc);
  uint8_t op1;
  uint8_t op2;

  if(instruction == 0x20)
  {
    pc++;
    op1 = _memory->read(pc);
    pc++;
    op2 = _memory->read(pc);

    _memory->write(sp--, pc);
    pc = op1 | (op2 << 8);

    return 6;
  }

  // LDX; Imm
  if(instruction == 0xA2)
  {
    pc++;
    op1 = _memory->read(pc);
    reg_x = op1;

    if(reg_x == 0)
      status.Z = 1;

    if(reg_x & 0x80)
      status.N = 1;

    pc++;

    return 2;
  }
  // LDX; ZP
  if(instruction == 0xA6)
  {
    pc++;
    op1 = _memory->read(pc);
    reg_x = _memory->read(op1);

    if(reg_x == 0)
      status.Z = 1;

    if(reg_x & 0x80)
      status.N = 1;

    pc++;

    return 3;
  }
  // LDX; ZP,Y
  if(instruction == 0xB6)
  {
    pc++;
    op1 = _memory->read(pc);
    reg_x = _memory->read(op1+reg_y);

    if(reg_x == 0)
      status.Z = 1;

    if(reg_x & 0x80)
      status.N = 1;

    pc++;

    return 4;
  }
  // LDX; Abs
  if(instruction == 0xAE)
  {
    pc++;
    op1 = _memory->read(pc);
    pc++;
    op2 = _memory->read(pc);
    reg_x = _memory->read(op1 | (op2 << 8));

    if(reg_x == 0)
      status.Z = 1;

    if(reg_x & 0x80)
      status.N = 1;

    pc++;

    return 4;
  }
  // LDX; Abs,Y
  if(instruction == 0xBE)
  {
    pc++;
    op1 = _memory->read(pc);
    pc++;
    op2 = _memory->read(pc);
    reg_x = _memory->read(reg_y + (op1 | (op2 << 8)));

    if(reg_x == 0)
      status.Z = 1;

    if(reg_x & 0x80)
      status.N = 1;

    pc++;

    return 4;
  }

  pc++;
  return 2;
}
