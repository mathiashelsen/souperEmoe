#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <thread>

#include <unistd.h>

#include "aux/fifo.hpp"
#include "aux/render_X11.hpp"
#include "aux/c64_constants.hpp"

void CreateTrueColorImage(fifo<unsigned char*>* videoStream)
{
    int i, j;
    //unsigned char *image32=(unsigned char *)malloc(XSIZE*YSIZE*4);
    unsigned char *p=(unsigned char *)malloc(XSIZE*YSIZE*4);
    for(i=0; i<XSIZE; i++)
    {
        for(j=0; j<YSIZE; j++)
        {
            if((i<256)&&(j<256))
            {
                *p++=rand()%256; // blue
                *p++=rand()%256; // green
                *p++=rand()%256; // red
            }
            else
            {
                *p++=i%256; // blue
                *p++=j%256; // green
                if(i<256)
                    *p++=i%256; // red
                else if(j<256)
                    *p++=j%256; // red
                else
                    *p++=(256-j)%256; // red
            }
            p++;
        }
    }
    videoStream->push(p);
}

int main(int argc, char **argv)
{
  fifo<unsigned char *> videoStream;
  render_X11 videoOutput = render_X11( &videoStream );
  videoOutput.run();
  //std::thread videoThread( &render_X11::run, &videoOutput );


  while(true)
  {
    // Here we should do actual work ofcourse
    CreateTrueColorImage( &videoStream );
    usleep(FRAMETIME);
  }

  return EXIT_SUCCESS;
}
