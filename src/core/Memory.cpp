#include "Memory.hpp"

Memory::Memory(int nBytes)
{
  ram     = (uint8_t *)malloc(nBytes);
  ramSize = nBytes;

  memset(ram, 0, nBytes);

  std::ifstream fileStream;

  fileStream.open(charROM_Name, std::ifstream::in);
  fileStream.get((char *)(ram + DEFAULT_CHAR_ROM_BASE_ADDR), 4096);
  fileStream.close();
  for(int i = 0; i < 2024; i++)
  {
    ram[i+DEFAULT_SCREEN_RAM_BASE_ADDR] = 32; //Empty char
  }

  fileStream.open(executableName, std::ifstream::in);
  fileStream.get((char*)(ram+0xc000), 1024);
  fileStream.close();

  // Set the PC at RST to 0xC000
  ram[RST_VECTOR]   = 0x00;
  ram[RST_VECTOR+1] = 0xC0;
  ram[IRQ_VECTOR]   = 0x00;
  ram[IRQ_VECTOR+1] = 0xCF;

}

Memory::Memory(int nBytes, const char* objectCodeFilename)
{
  ram     = (uint8_t *)malloc(nBytes);
  ramSize = nBytes;

  memset(ram, 0, nBytes);

  std::ifstream fileStream;

  fileStream.open(charROM_Name, std::ifstream::in);
  fileStream.get((char *) (ram + DEFAULT_CHAR_ROM_BASE_ADDR), 4096);
  fileStream.close();
  for(int i = 0; i < 2024; i++)
  {
    ram[i+DEFAULT_SCREEN_RAM_BASE_ADDR] = 32; //Empty char
  }

  //std::ifstream codeStream(objectCodeFilename, std::ios::binary);
  //codeStream.get((char *)(ram+0xc000), 2048);
  //std::cout << "Total bytes read: " << codeStream.gcount() << std::endl;
  //codeStream.close();

  FILE * fs = fopen(objectCodeFilename, "rb");
  fseek(fs, 0, SEEK_END);
  int fSize = ftell(fs);
  rewind(fs);
  std::cout << "C says there are " << fSize << " byte to be read" << std::endl;
  fread(ram+0xC000, fSize, 1, fs);
  fclose(fs);

  // Set the PC at RST to 0xC000
  ram[RST_VECTOR]   = 0x00;
  ram[RST_VECTOR+1] = 0xC0;
  ram[IRQ_VECTOR]   = 0x00;
  ram[IRQ_VECTOR+1] = 0xCF;
}

Memory::~Memory()
{
  free(ram);
}

uint8_t Memory::read(int addr)
{
  //printf("Reading value 0x%02X from 0x%04X\n", (uint8_t) ram[addr], (uint16_t) addr);
  if(addr <= ramSize)
  {
    return ram[addr];
  }
  else
  {
    return (uint8_t) 0;
  }
}

void Memory::write(int addr, uint8_t data)
{
  //printf("Writing value 0x%02X to address %04X\n", (uint8_t) data, (uint16_t) addr);
  if(addr < ramSize)
    ram[addr] = data;
}
