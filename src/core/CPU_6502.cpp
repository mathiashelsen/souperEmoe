#include "CPU_6502.hpp"

CPU_6502::CPU_6502(Memory *  memory) : CPU(memory)
{
  this->reset();
}

void CPU_6502::reset(void)
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

int CPU_6502::runNextOperation()
{
  uint8_t   instruction = _memory->read(pc);
  uint8_t   op1         = _memory->read(pc+1);
  uint8_t   op2         = _memory->read(pc+2);
  uint16_t  address     = 0;
  uint8_t   operand     = 0;
  int       retVal      = 0;

  instrType decodInstr  = instr[instruction];

  pc++;

  switch(decodInstr._addrMode)
  {
    case _ZPX_: // (ZP, X) -- Good
      address = _memory->read(op1 + reg_x) | (_memory->read(op1 + reg_x + 1) << 8);
      operand = _memory->read(address);
      pc      += 1;
      break;
    case ZP: // ZP -- Good
      operand = _memory->read(op1);
      pc      += 1;
      break;
    case Imm: // Imm -- Good
      operand = op1;
      pc      += 1;
      break;
    case Abs: // Abs -- Good
      address = op1 | (op2 << 8);
      operand = _memory->read(address);
      pc     += 2;
      break;
    case _ZP_Y: // (ZP), y -- Good
      address = _memory->read(op1);
      address = (_memory->read(address) | (_memory->read(address+1) << 8)) + reg_y;
      operand = _memory->read(address);
      pc      += 1;
      break;
    case ZPX: // ZP, x -- Good
      operand = _memory->read(op1 + reg_x); 
      pc      += 1;
      break;
    case AbsY: // Abs, y -- Good
      address = (op1 | (op2 << 8)) + reg_y;
      operand = _memory->read(address);
      pc      += 2;
      break;
    case AbsX: // Abs, x -- Good
      address = (op1 | (op2 << 8)) + reg_x;
      operand = _memory->read(address);
      pc      += 2;
      break;
    case Impl:
      break;
  }

  switch(decodInstr._opCode)
  {
      case NOP:
        std::cout << "NOP" << std::endl;
        break;
      case LDA:
        std::cout << "LDA" << std::endl;
        acc = operand;
        break;
      default:
        std::cout << "Unknown instruction" << std::endl;
        break;
  }

  return 2;
}
