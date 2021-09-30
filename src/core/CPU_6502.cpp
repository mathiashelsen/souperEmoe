#include "CPU_6502.hpp"

CPU_6502::CPU_6502(Memory *  memory) : CPU(memory)
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

  uint8_t a, b, c;
  a = instruction & 0b11;
  b = (instruction >> 2) & 0b111;
  c = (instruction >> 5) & 0b111;

  pc++;

  switch(c)
  {
    case(0b01):
      switch(b)
      {
        case 0b000: // (ZP, X) -- Good
          address = _memory->read(op1 + reg_x) | (_memory->read(op1 + reg_x + 1) << 8);
          operand = _memory->read(address);
          pc      += 1;
          break;
        case 0b001: // ZP -- Good
          operand = _memory->read(op1);
          pc      += 1;
          break;
        case 0b010: // Imm -- Good
          operand = op1;
          pc      += 1;
          break;
        case 0b011: // Abs -- Good
          address = op1 | (op2 << 8);
          operand = _memory->read(address);
          pc     += 2;
          break;
        case 0b100: // (ZP), y -- Good
          address = _memory->read(op1);
          address = (_memory->read(address) | (_memory->read(address+1) << 8)) + reg_y;
          operand = _memory->read(address);
          pc      += 1;
          break;
        case 0b101: // ZP, x -- Good
          operand = _memory->read(op1 + reg_x); 
          pc      += 1;
          break;
        case 0b110: // Abs, y -- Good
          address = (op1 | (op2 << 8)) + reg_y;
          operand = _memory->read(address);
          pc      += 2;
          break;
        case 0b111: // Abs, x -- Good
          address = (op1 | (op2 << 8)) + reg_x;
          operand = _memory->read(address);
          pc      += 2;
          break;
      }

      switch(a)
      {
        case 0b000: // ORA
          acc = acc | operand;
          break;
        case 0b001: // AND
          acc = acc & operand;
          break;
        case 0b010: // EOR
          acc = acc ^ operand;
          break;
        case 0b011: // ADC
          acc = acc + operand;
          break;
        case 0b100: // STA;
          _memory->write(operand, acc);
          break;
        case 0b101: // LDA;
          acc = operand;
          break;
        case 0b110: // CMP;
          // TODO
          break;
        case 0b111: // SBC;
          acc = acc - operand;
          break;
        default:
          std::cout << "Unknown instruction " << std::endl;
      }
    default:
      std::cout << "Unknown instruction" << std::endl;
  }


  //std::cout << "Unknown instruction" << std::endl;
  pc++;
  return 2;
}
