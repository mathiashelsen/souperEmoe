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

  //std::cout << pc << ": ";

  if(instruction == 0x20)
  {
    //std::cout << "JSR" << std::endl;
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
    //std::cout << "LDX" << std::endl;
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

  // LDA, ZP,X
  if(instruction == 0xB5)
  {
    //std::cout << "LDA" << std::endl;
    pc++;
    op1 = _memory->read(pc);
    acc = _memory->read(op1+reg_x);

    if(acc == 0)
      status.Z = 1;

    if(acc & 0x80)
      status.N = 1;

    pc++;

    return 4;
  }

  // STA, Abs, X
  if(instruction == 0x9D)
  { 
    //std::cout << "STA" << std::endl;
    pc++;
    op1 = _memory->read(pc);
    pc++;
    op2 = _memory->read(pc);
    _memory->write(reg_x + (op1 | (op2 << 8)), acc);

    pc++;
    return 5;
  }

  if(instruction == 0xE8)
  {
    //std::cout << "INX" << std::endl;
    reg_x = (reg_x + 1) % 0xff;
 
    if(reg_x == 0)
      status.Z = 1;

    if(reg_x & 0x80)
      status.N = 1;

    pc++;

    return 2;
  }

  if(instruction == 0xE0)
  {
    //std::cout << "CPX" << std::endl;
    pc++;
    op1 = _memory->read(pc);

    if(reg_x >= op1)
    {
      status.C = 1;
      status.Z = 0;
    }

    if(reg_x == op1)
    {
      status.C = 0;
      status.Z = 1;
      status.N = 0;
    }

    if((uint8_t) reg_x < op1)
    {
      status.C = 0;
      status.Z = 0;
    }

    pc++;

    return 2;
  }

  if(instruction == 0xD0)
  {
    //std::cout << "BNE" << std::endl;
    pc++;
    op1 = _memory->read(pc);
    if(status.Z == 0)
    {
      pc = pc + (char) op1 + 1;
    }
    else
    {
      pc++;
    }

    return 2;
  }

  if(instruction == 0x4C)
  {
    //std::cout << "JMP" << std::endl;
    pc++;
    op1 = _memory->read(pc);
    pc++;
    op2 = _memory->read(pc);

    pc = op1 | (op2 << 8);

    return 6;
    
  }

  //std::cout << "Unknown instruction" << std::endl;
  pc++;
  return 2;
}
