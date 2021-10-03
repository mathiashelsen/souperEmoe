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
  pc      = _memory->read(RST_VECTOR) | (_memory->read(RST_VECTOR+1) << 8);
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
  int       retVal      = 2;

  instrType decodInstr  = instr[instruction];

  pc++;

  printf("PC = %04X\n", (uint16_t) pc);

  switch(decodInstr._addrMode)
  {
    case _ZPX_: // (ZP, X) -- Good
      address = _memory->read(op1 + reg_x) | (_memory->read(op1 + reg_x + 1) << 8);
      operand = _memory->read(address);
      pc      += 1;
      break;
    case ZP: // ZP -- Good
      address = op1;
      operand = _memory->read(address);
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
      address = _memory->read(op1) | (_memory->read(op1 + 1) << 8);
      address += (uint8_t) reg_y; // Required for add-with-carry, y=0xEF is positive and increases address
      operand = _memory->read(address);
      pc      += 1;
      break;
    case ZPX: // ZP, x -- Good
      address = op1 + reg_x; // Add-without-carry, e.g. x=0xEF is negative and decreases address
      operand = _memory->read(op1 + reg_x); 
      pc      += 1;
      break;
    case ZPY: // ZP, x -- Good
      operand = _memory->read(op1 + reg_y); 
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
        break;
      case LDA:
        acc = operand;
        this->updateFlagsNZ(acc);
        break;
      case LDX:
        reg_x = operand;
        this->updateFlagsNZ(reg_x);
        break;
      case LDY:
        reg_y = operand;
        this->updateFlagsNZ(reg_y);
        break;
      case STA:
        _memory->write(address, acc);
        break;
      case INC:
        _memory->write(address, operand+1);
        this->updateFlagsNZ(operand+1);
        break;
      case DEC:
        _memory->write(address, operand-1);
        this->updateFlagsNZ(operand-1);
        break;
      default:
        std::cout << "Unknown instruction" << std::endl;
        break;
  }

  return retVal;
}

void CPU_6502::updateFlagsNZ(char regVal)
{
  if(regVal < 0)
    status.N = 1;
  else
    status.N = 0;


  if(regVal == 0)
    status.Z = 1;
  else
    status.Z = 0;
}
