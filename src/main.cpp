#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <thread>

#include <unistd.h>
#include <cstdio>

#include "aux/fifo.hpp"
#include "aux/render_X11.hpp"
#include "aux/c64_constants.hpp"

#include "core/Computer.hpp"

#include "unittests/lda_vfct.hpp"
#include "unittests/adc_vfct.hpp"
#include "unittests/int_vfct.hpp"

#include <algorithm>

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char **argv)
{
  int   OSR = 3;
  char* fileName;

  if(cmdOptionExists(argv, argv+argc, "--test"))
  {
    adc_runtest();
    lda_runtest();
    int_runtest();

    return EXIT_SUCCESS;
  }

  if(cmdOptionExists(argv, argv+argc, "--file"))
  {
    fileName = getCmdOption(argv, argv+argc, "--file");
  }
  else
  {
    printf("Error: if not running a test, a file is required. Specify with --file [fileName]\n");
    return EXIT_SUCCESS;
  }

  if(cmdOptionExists(argv, argv+argc, "--OSR"))
  {
    OSR = getCmdOption(argv, argv+argc, "--OSR");
  }

  fifo<unsigned char *> videoStream;
  keystream             keyStream;
  render_X11 videoOutput = render_X11( &videoStream, OSR, &keyStream);
  std::thread videoThread( &render_X11::run, &videoOutput );

  Computer *computer = new Computer(&videoStream, OSR, &keyStream, fileName);
  computer->run();

  videoThread.join();

  return EXIT_SUCCESS;
}
