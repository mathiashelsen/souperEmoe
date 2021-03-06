#ifndef _COMPUTER_HPP
#define _COMPUTER_HPP

#include "../aux/fifo.hpp"
#include "../aux/keystream.hpp"
#include "../aux/c64_constants.hpp"

#include "MemoryController.hpp"
#include "Memory.hpp"
//#include "DummyMemory.hpp"
#include "CPU.hpp"
#include "CPU_6502.hpp"
#include "Video.hpp"
#include "VICII.hpp"
//#include "BouncingBallVideo.hpp"

#include <chrono>

#include <unistd.h>

class Computer
{
  protected:
    keystream*          _keyStream;
    Video*              video;
    MemoryController*   memoryCtl;
    bool                reset;
    int                 _OSR;

    std::chrono::high_resolution_clock::time_point startOfFrame;
    std::chrono::high_resolution_clock::time_point endOfFrame;
  public:
    Memory*             memory;
    CPU_6502*           cpu;
    Computer(fifo<unsigned char*>* videoStream, int OSR, keystream* keyStream);
    Computer(fifo<unsigned char*>* videoStream, int OSR, keystream* keyStream, const char* objectCodeFilename);
    void run();
    void run(int nInstructions);
    void run(int nInstructions, int IRQ);
};

#endif
