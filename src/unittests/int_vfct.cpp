#include "int_vfct.hpp"

void int_runtest(void)
{
  //int errorCount = 0;
  int OSR        = 3;
  //uint8_t accVal    = 0;
  //statusType stat;

  fifo<unsigned char *> videoStream;

  Computer *computer = new Computer(&videoStream, OSR, NULL, "software/int_test.o");
 
  printf("First IRQ instruction: 0x%02X\n", computer->memory->read(0xcf00));

  for(int i = 0; i < 100; i++)
  {
    if(i == 59){
      computer->run(1, 1);
    } else {
      computer->run(1);
    }
    printf("Step %02d - X=%02d, Y=%02d\n", i, (uint8_t) computer->cpu->getX(), (uint8_t) computer->cpu->getY());
  }
}
