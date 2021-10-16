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

  _keyStream = NULL;
}

Memory::Memory(int nBytes, const char* objectCodeFilename, keystream* keyStream)
{
  ram         = (uint8_t *)malloc(nBytes);
  ramSize     = nBytes;
  _keyStream  = keyStream;

  memset(ram, 0, nBytes);


  charROM     = (uint8_t *)malloc(4096);
  std::ifstream fileStream;

  fileStream.open(charROM_Name, std::ifstream::in);
  fileStream.get((char *) charROM, 4096);
  fileStream.close();

  //for(int i = 0; i < 2024; i++)
  //{
  //  ram[i+DEFAULT_SCREEN_RAM_BASE_ADDR] = 32; //Empty char
  //}

  //std::ifstream codeStream(objectCodeFilename, std::ios::binary);
  //codeStream.get((char *)(ram+0xc000), 2048);
  //std::cout << "Total bytes read: " << codeStream.gcount() << std::endl;
  //codeStream.close();

  //////////////////////
  //                  //
  // LOAD OBJECT CODE //
  //                  //
  //////////////////////
  FILE * fs = fopen(objectCodeFilename, "rb");
  fseek(fs, 0, SEEK_END);
  int fSize = ftell(fs);
  rewind(fs);
  std::cout << "C says there ares " << fSize << " byte to be read" << std::endl;
  fread(ram+0xC000, fSize, 1, fs);
  fclose(fs);

  //////////////////////
  //                  //
  // LOAD KERNAL CODE //
  //                  //
  //////////////////////
  kernalROM   = (uint8_t *) malloc(8192);
  fs          = fopen("software/kernal.o", "rb");
  fseek(fs, 0, SEEK_END);
  fSize       = ftell(fs);
  rewind(fs);
  std::cout << "C says there ares " << fSize << " byte to be read" << std::endl;
  fread(kernalROM, fSize, 1, fs);
  fclose(fs);


  colorRAM    = (uint8_t *) malloc(1024);
}

Memory::~Memory()
{
  free(ram);
  free(charROM);
  free(kernalROM);
  free(colorRAM);
}

uint8_t Memory::read(uint16_t addr)
{

  if(addr <= ramSize)
  {
    return ram[addr];
  }
  else
  {
    return (uint8_t) 0;
  }
}

uint8_t Memory::read_char_rom(uint16_t addr)
{
  if(addr < 4096)
  {
    return charROM[addr];
  }
  else
  {
    return 0;
  }
}

uint8_t Memory::read_kernal_rom(uint16_t addr)
{

  // A request is made for SCNKEY KERNAL routine
  // This entire routine is now done inside "Memory"
  // and the kernal is a NOP and RTS
  if(addr == (0xff9f - DEFAULT_KERNAL_ROM_BASE_ADDR))
  {
    char* keysPressed;
    int   nKeysPressed;
    keysPressed = _keyStream->getAllPressed(nKeysPressed);
    ram[0xC6] = (uint8_t) nKeysPressed;
    if(nKeysPressed)
    {
      for(int i = 0; i < nKeysPressed && i < 10; i++)
      {
        ram[0x277 + (uint8_t)i] = (uint8_t) keysPressed[i];
      }

      delete keysPressed;
    }
  }

  if(addr < 8192)
  {
    return kernalROM[addr];
  }
  else
  {
    return 0;
  }
}

uint8_t Memory::read_color_ram(uint16_t addr)
{
  if(addr < 1024)
  {
    return colorRAM[addr];
  }

  return 0;
}


void Memory::write(uint16_t addr, uint8_t data)
{
  if(addr < ramSize){
    ram[addr] = data;
  }
}


void Memory::write_color_ram(uint16_t addr, uint8_t data)
{
  if(addr < 1024)
  {
    colorRAM[addr] = data;
  }
}
