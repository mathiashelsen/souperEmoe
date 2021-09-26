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
  fifo<unsigned char *> videoStream;
  render_X11 videoOutput = render_X11( &videoStream );
  std::thread videoThread( &render_X11::run, &videoOutput );

  Computer *computer = new Computer(&videoStream);
  computer->run();  

  delete computer;
  return EXIT_SUCCESS;
}

//void CreateTrueColorImage(fifo<unsigned char*>* videoStream)
//{
//    int i, j;
//    unsigned char *p = NULL;
//    p = (unsigned char *)malloc(XSIZE*YSIZE*4);
//    unsigned char *image = p;
//    for(i=0; i<XSIZE; i++)
//    {
//        for(j=0; j<YSIZE; j++)
//        {
//            if((i<256)&&(j<256))
//            {
//                //*p++=100;
//                //*p++=100;
//                //*p++=100;
//
//                *p++=rand()%256; // blue
//                *p++=rand()%256; // green
//                *p++=rand()%256; // red
//            }
//            else
//            {
//                *p++=i%256; // blue
//                *p++=j%256; // green
//                if(i<256)
//                    *p++=i%256; // red
//                else if(j<256)
//                    *p++=j%256; // red
//                else
//                    *p++=(256-j)%256; // red
//            }
//            p++;
//        }
//    }
//    videoStream->push(image);
//}
