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
  uint16_t  result      = 0;
  int       retVal      = 2;
  uint8_t   temp        = 0;

  instrType decodInstr  = instr[instruction];

  printf("PC = %04X\n", (uint16_t) pc);
  pc++;


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
      address = (op1 | (op2 << 8)) + (uint8_t) reg_y;
      operand = _memory->read(address);
      pc      += 2;
      break;
    case AbsX: // Abs, x -- Good
      address = (op1 | (op2 << 8)) + (uint8_t) reg_x;
      operand = _memory->read(address);
      pc      += 2;
      break;
    case Acc:
      operand = (uint8_t) acc;
      break;
    case Ind:
      address = op1 | (op2 << 8);
      // TODO: The line below is not correct, but I am lazy. If address = 0x03FF
      // it should compose the new address out of (0x03FF) | (0x0300), i.e.
      // it should overflow.
      address = _memory->read(address) | (_memory->read(address+1) << 8);
      pc     += 2;
      break;
    case Impl:
      break;
  }

  switch(decodInstr._opCode)
  {
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
      case TAX:
        reg_x = acc;
        this->updateFlagsNZ(reg_x);
        break;
      case TAY:
        reg_y = acc;
        this->updateFlagsNZ(reg_y);
        break;
      case TXA:
        acc = reg_x;
        this->updateFlagsNZ(acc);
        break;
      case TYA:
        acc = reg_y;
        this->updateFlagsNZ(acc);
        break;
      case DEX:
        reg_x--;
        this->updateFlagsNZ(reg_x);
        break;
      case DEY:
        reg_y--;
        this->updateFlagsNZ(reg_y);
        break;
      case INX:
        reg_x++;
        this->updateFlagsNZ(reg_x);
        break;
      case INY:
        reg_y++;
        this->updateFlagsNZ(reg_y);
        break;
      case ADC:
        if(status.D == 0)
        {
          result          = ((uint8_t) acc + operand) + status.C;
          status.C        = (result & 0x100) >> 8;
          status.V        = ((acc ^ result) & (operand ^ result) & 0x80) >> 7;
          acc             = result & 0xff;
          this->updateFlagsNZ(acc);
        }else{
          // Its NMOS 6502 mode, to NV(Z) are invalid
          result          = ((uint8_t) acc + operand) + status.C;
          status.V        = ((acc ^ result) & (operand ^ result) & 0x80) >> 7;
          this->updateFlagsNZ(acc);

          // Correct the output
          if( (result & 0x0f) > 0x09 )
            result += 0x06;
          if( (result & 0xf0) > 0x90) // 9 << 4
            result += 0x60;

          status.C        = result > 0x99 ? 1 : 0;
          acc             = result & 0xff;
        }
        break;
      case SBC:
        if(status.D == 0)
        {
          result          = ((uint8_t) acc + (~operand)) + status.C;
          status.C        = (result & 0x100) >> 8;
          status.V        = ((acc ^ result) & ((~operand) ^ result) & 0x80) >> 7;
          acc             = result & 0xff;
          this->updateFlagsNZ(acc);
        }else{
          // Its NMOS 6502 mode, to NV(Z) are invalid
          result          = ((uint8_t) acc + (~operand)) + status.C;
          status.V        = ((acc ^ result) & ((~operand) ^ result) & 0x80) >> 7;
          this->updateFlagsNZ(acc);

          // Correct the output
          if( (result & 0x0f) > 0x09 )
            result += 0x06;
          if( (result & 0xf0) > 0x90) // 9 << 4
            result += 0x60;

          status.C        = result > 0x99 ? 1 : 0;
          acc             = result & 0xff;
        }
        break;
      case CLD:
        status.D = 0;
        break;
      case SED:
        status.D = 1;
        break;
      case CLC:
        status.C = 0;
        break;
      case SEC:
        status.C = 1;
        break;
      case CLV:
        status.V = 0;
        break;
      case CLI:
        status.I = 0;
        break;
      case SEI:
        status.I = 1;
        break;
      case CMP:
        status.C = ((uint8_t) acc) >  operand ? 1 : 0;
        status.Z = ((uint8_t) acc) == operand ? 1 : 0;
        status.N = ((uint8_t) acc) <  operand ? 1 : 0;
        break;
      case CPX:
        status.C = ((uint8_t) reg_x) >  operand ? 1 : 0;
        status.Z = ((uint8_t) reg_x) == operand ? 1 : 0;
        status.N = ((uint8_t) reg_x) <  operand ? 1 : 0;
        break;
      case CPY:
        status.C = ((uint8_t) reg_y) >  operand ? 1 : 0;
        status.Z = ((uint8_t) reg_y) == operand ? 1 : 0;
        status.N = ((uint8_t) reg_y) <  operand ? 1 : 0;
        break;
      case BIT:
        status.N = (operand >> 6) & 0x01;
        status.V = (operand >> 5) & 0x01;
        status.Z = (operand & acc) == 0 ? 1 : 0;
        break;
      case BNE:
        if(status.Z == 0)
        {
          pc += (char) operand;
        }
        break;
      case BEQ:
        if(status.Z == 1)
        {
          pc += (char) operand;
        }
        break;
      case BPL:
        if(status.N == 0)
        {
          pc += (char) operand;
        }
        break;
      case BMI:
        if(status.N == 1)
        {
          pc += (char) operand;
        }
        break;
      case BVC:
        if(status.V == 0)
        {
          pc += (char) operand;
        }
        break;
      case BVS:
        if(status.V == 1)
        {
          pc += (char) operand;
        }
        break;
      case BCC:
        if(status.C == 0)
        {
          pc += (char) operand;
        }
        break;
      case BCS:
        if(status.C == 1)
        {
          pc += (char) operand;
        }
        break;
      case AND:
        acc = acc & operand;
        this->updateFlagsNZ(acc);
        break;
      case EOR:
        acc = acc ^ operand;
        this->updateFlagsNZ(acc);
        break;
      case ORA:
        acc = acc | operand;
        this->updateFlagsNZ(acc);
        break;
      case ASL:
        status.C = (acc >> 7);
        acc = acc << 1;
        this->updateFlagsNZ(acc);
        break;
      case LSR:
        status.C = acc & 1;
        acc = ((uint8_t) acc) >> 1;
        this->updateFlagsNZ(acc);
        break;
      case ROL:
        temp = (acc >> 7);
        acc = (acc << 1) | status.C;
        status.C = temp;
        this->updateFlagsNZ(acc);
        break;
      case ROR:
        temp = acc & 1;
        acc = (((uint8_t) acc) >> 1) | (status.C << 7);
        status.C = temp;
        this->updateFlagsNZ(acc);
        break;
      case JMP:
        pc = address;
        break;
      case JSR:
        _memory->write((0x100 | sp), pc & 0xff);
        sp--;
        _memory->write((0x100 | sp), (pc >> 8) & 0xff);
        sp--;
        pc = address;
        break;
      case RTS:
        sp++;
        pc = _memory->read((0x100 | sp)) << 8;
        sp++;
        pc = pc | _memory->read((0x100 | sp));
        break;
      case RTI:
        sp++;
        this->setStatusFlags(_memory->read((0x100 | sp)));
        sp++;
        pc = _memory->read((0x100 | sp)) << 8;
        sp++;
        pc = pc | _memory->read((0x100 | sp));
        break;
      case PHA:
        _memory->write(0x100 | sp, acc);
        sp--;
        break;
      case PHP:
        _memory->write(0x100 | sp, this->getStatusFlags());
        sp--;
        break;
      case TXS:
        sp = (uint8_t) reg_x;
        break;
      case PLA:
        sp++;
        acc = _memory->read(0x100 | sp);
        this->updateFlagsNZ(acc);
        break;
      case PLP:
        sp++;
        this->setStatusFlags(_memory->read(0x100 | sp));
        break;
      case TSX:
        reg_x = (char) sp;
        this->updateFlagsNZ(reg_x);
        break;
      case BRK:
        status.B = 1;
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
    
void CPU_6502::setStatusFlags(uint8_t val)
{
  uint8_t tmp = val;

  status.C = tmp & 1;
  tmp = tmp >> 1;
  status.Z = tmp & 1;
  tmp = tmp >> 1;
  status.I = tmp & 1;
  tmp = tmp >> 1;
  status.D = tmp & 1;
  tmp = tmp >> 1;
  status.B = tmp & 1;
  tmp = tmp >> 2;
  status.V = tmp & 1;
  tmp = tmp >> 1;
  status.N = tmp & 1;
}

uint8_t CPU_6502::getStatusFlags()
{
  uint8_t retVal = 0;

  retVal |=  status.C;
  retVal |= (status.Z << 1);
  retVal |= (status.I << 2);
  retVal |= (status.D << 3);
  retVal |= (status.B << 4);
  retVal |= (status.V << 6);
  retVal |= (status.N << 7);

  return retVal;
}
