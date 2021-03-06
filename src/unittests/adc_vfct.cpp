#include "adc_vfct.hpp"

void adc_runtest(void)
{
  int errorCount = 0;
  int OSR        = 3;
  uint8_t accVal    = 0;
  statusType stat;

  fifo<unsigned char *> videoStream;

  Computer *computer = new Computer(&videoStream, OSR, NULL, "software/adc_test.o");

  computer->run(2);
  stat    = computer->cpu->getStatus();
  accVal  = (uint8_t) computer->cpu->getAcc();

  if(accVal != 0xA0 || stat.N != 1 || stat.V != 1)
  {
    errorCount++;
    printf("Error: acc value = 0x%02X, N=%d, Z=%d, V=%d, C=%d\n", accVal, stat.N, stat.Z, stat.V, stat.C);
  }

  computer->run(1);
  stat    = computer->cpu->getStatus();
  accVal  = (uint8_t) computer->cpu->getAcc();
  if(accVal != 0x00 || stat.N != 0 || stat.Z != 1 || stat.C != 1)
  {
    errorCount++;
    printf("Error: acc value = 0x%02X, N=%d, Z=%d, V=%d, C=%d\n", accVal, stat.N, stat.Z, stat.V, stat.C);
  }

  computer->run(2);
  stat    = computer->cpu->getStatus();
  accVal  = (uint8_t) computer->cpu->getAcc();
  if(accVal != 0x0B || stat.N != 0 || stat.Z != 0 || stat.C != 1)
  {
    errorCount++;
    printf("Error: acc value = 0x%02X, N=%d, Z=%d, V=%d, C=%d\n", accVal, stat.N, stat.Z, stat.V, stat.C);
  }

  computer->run(4);
  stat    = computer->cpu->getStatus();
  accVal  = (uint8_t) computer->cpu->getAcc();
  if(accVal != 0x93 || stat.C != 0)
  {
    errorCount++;
    printf("Error: acc value = 0x%02X, N=%d, Z=%d, V=%d, C=%d\n", accVal, stat.N, stat.Z, stat.V, stat.C);
  }

  computer->run(1);
  stat    = computer->cpu->getStatus();
  accVal  = (uint8_t) computer->cpu->getAcc();
  if(accVal != 0x00 || stat.C != 1)
  {
    errorCount++;
    printf("Error: acc value = 0x%02X, N=%d, Z=%d, V=%d, C=%d\n", accVal, stat.N, stat.Z, stat.V, stat.C);
  }

  computer->run(3);
  stat    = computer->cpu->getStatus();
  accVal  = (uint8_t) computer->cpu->getAcc();
  if(accVal != 0x50 || stat.C != 0)
  {
    errorCount++;
    printf("Error: acc value = 0x%02X, N=%d, Z=%d, V=%d, C=%d\n", accVal, stat.N, stat.Z, stat.V, stat.C);
  }

  computer->run(31); // Count up to 10 in regX 
  if((uint8_t) computer->cpu->getX() != 0x0A)
  {
    errorCount++;
    printf("Error: regX value = 0x%02X, N=%d, Z=%d, V=%d, C=%d\n", (uint8_t) computer->cpu->getX(), stat.N, stat.Z, stat.V, stat.C);

  }

  computer->run(1); // Do not branch and set to 0xEF
  if((uint8_t) computer->cpu->getX() != 0xEF)
  {
    errorCount++;
    printf("Error: regX value = 0x%02X, N=%d, Z=%d, V=%d, C=%d\n", (uint8_t) computer->cpu->getX(), stat.N, stat.Z, stat.V, stat.C);

  }

  computer->run(3);
  stat    = computer->cpu->getStatus();
  accVal  = (uint8_t) computer->cpu->getAcc();
  if(accVal != 0x81 || stat.C != 0)
  {
    errorCount++;
    printf("Error: acc value = 0x%02X, N=%d, Z=%d, V=%d, C=%d\n", accVal, stat.N, stat.Z, stat.V, stat.C);
  }
  computer->run(1);

  stat    = computer->cpu->getStatus();
  accVal  = (uint8_t) computer->cpu->getAcc();
  if(accVal != 0x02 || stat.C != 1)
  {
    errorCount++;
    printf("Error: acc value = 0x%02X, N=%d, Z=%d, V=%d, C=%d\n", accVal, stat.N, stat.Z, stat.V, stat.C);
  }

  computer->run(1);
  stat    = computer->cpu->getStatus();
  accVal  = (uint8_t) computer->cpu->getAcc();
  if(accVal != 0x81 || stat.C != 0)
  {
    errorCount++;
    printf("Error: acc value = 0x%02X, N=%d, Z=%d, V=%d, C=%d\n", accVal, stat.N, stat.Z, stat.V, stat.C);
  }
  computer->run(1);
  stat    = computer->cpu->getStatus();
  accVal  = (uint8_t) computer->cpu->getAcc();
  if(accVal != 0x40 || stat.C != 1)
  {
    errorCount++;
    printf("Error: acc value = 0x%02X, N=%d, Z=%d, V=%d, C=%d\n", accVal, stat.N, stat.Z, stat.V, stat.C);
  }

  computer->run(2);
  if(computer->cpu->getX() != 0x00)
  {
    errorCount++;
    printf("Error, reg_x value = 0x%02X\n", computer->cpu->getX());

  }

  computer->run(4);
  if(computer->cpu->getX() != 0x03)
  {
    errorCount++;
    printf("Error, reg_x value = 0x%02X\n", computer->cpu->getX());
  }

  computer->run(2);
  if(computer->cpu->getAcc() != 0x03)
  {
    errorCount++;
    printf("Error acc value = 0x%02X\n", computer->cpu->getAcc());
  }

  computer->run(7);
  if(computer->cpu->getAcc() != 0x13)
  {
    errorCount++;
    printf("Error acc value = 0x%02X\n", computer->cpu->getAcc());
  }

  computer->run(1);
  if(computer->cpu->getAcc() != 0x03)
  {
    errorCount++;
    printf("Error acc value = 0x%02X\n", computer->cpu->getAcc());
  }
  

  delete computer;

  if(errorCount == 0)
    printf("ADC Unit Test: PERFECT SCORE\n");
  else
    printf("ADC Unit Test: Total number of errors found: %d\n", errorCount);

}
