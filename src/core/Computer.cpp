#include "Computer.hpp"

Computer::Computer(fifo<unsigned char*>* videoStream, int OSR)
{
  memory  = new Memory(64000);
  cpu     = new CPU(memory);
  video   = new VICII(videoStream, memory, OSR);
  //video   = new BouncingBallVideo(videoStream, memory);
  //video   = new Video(videoStream, memory);
}

void Computer::run(void)
{
  int totalCycles = 0;
  int cpuCycles = 0;
  int avgFrameTime[100];
  int frameTimeIndex = 0;
  reset = false;

  while(!reset)
  {
    // Start the chronometer at the beginning of a new frame
    startOfFrame = std::chrono::high_resolution_clock::now();

    totalCycles -= CYCLES_PER_FRAME;

    while(totalCycles < CYCLES_PER_FRAME)
    {
      cpuCycles   =   cpu->runNextOperation();
      totalCycles +=  cpuCycles;
      totalCycles +=  video->runNextOperation(cpuCycles);
    }

    // Stop the chronometer when enough clock cycles have passed
    endOfFrame = std::chrono::high_resolution_clock::now();
    int frameTime = (int) std::chrono::duration_cast<std::chrono::microseconds>(endOfFrame - startOfFrame).count();

    avgFrameTime[frameTimeIndex] = frameTime;
    frameTimeIndex++;
    if(frameTimeIndex == 100)
    {
      int avgFrameTimeValue = 0;
      for(int i=0; i<100; i++)
      {
        avgFrameTimeValue += avgFrameTime[i];
      }

      std::cout << "Average time used for computation: " << (avgFrameTimeValue/100) << std::endl;
      std::cout << "Average load used for simulation : " << ((double)avgFrameTimeValue/100./(double)FRAMETIME) << std::endl;

      frameTimeIndex = 0;
    }

    // Go to sleep if required
    if(frameTime < FRAMETIME)
      usleep(FRAMETIME - frameTime);
  }
}
