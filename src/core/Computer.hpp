#ifndef _COMPUTER_HPP
#define _COMPUTER_HPP

#include "../aux/fifo.hpp"
#include "../aux/c64_constants.hpp"

#include "Memory.hpp"
#include "CPU.hpp"
#include "Video.hpp"
#include "VICII.hpp"
//#include "BouncingBallVideo.hpp"

#include <chrono>

#include <unistd.h>

class Computer
{
  protected:
    Memory* memory;
    CPU*    cpu;
    Video*  video;
    bool    reset;
    int     _OSR;

    std::chrono::high_resolution_clock::time_point startOfFrame;
    std::chrono::high_resolution_clock::time_point endOfFrame;
  public:
    Computer(fifo<unsigned char*>* videoStream, int OSR);
    void run();
};

#endif
