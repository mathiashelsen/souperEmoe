#include "lda_vfct.hpp"

void lda_runtest(void)
{
  int errorCount = 0;
  int OSR        = 3;

  fifo<unsigned char *> videoStream;
  render_X11 videoOutput = render_X11( &videoStream, OSR);
  std::thread videoThread( &render_X11::run, &videoOutput );

  Computer *computer = new Computer(&videoStream, OSR, "software/lda_test.o");
  computer->run(1);
  if((uint8_t) computer->cpu->getAcc() != 0xef)
  {
    errorCount++;
    printf("Acc = 0x%02X, expected 0xEF\n", (uint8_t) computer->cpu->getAcc());
  }

  delete computer;
  printf("Total number of errors found: %d\n", errorCount);
}
