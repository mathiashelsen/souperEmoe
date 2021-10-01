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


int main(int argc, char **argv)
{
  lda_runtest();

  return EXIT_SUCCESS;
}
