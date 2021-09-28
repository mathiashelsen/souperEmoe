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


int main(int argc, char **argv)
{
  int OSR = 2;
  fifo<unsigned char *> videoStream;
  render_X11 videoOutput = render_X11( &videoStream, OSR);
  std::thread videoThread( &render_X11::run, &videoOutput );

  Computer *computer = new Computer(&videoStream, OSR);
  computer->run();  

  delete computer;
  return EXIT_SUCCESS;
}
