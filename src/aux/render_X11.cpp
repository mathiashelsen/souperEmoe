#include "render_X11.hpp"

// For debug
unsigned char *p;
volatile int idx;

render_X11::render_X11(fifo<unsigned char *> *Input_FIFO)
{
  _videoStream = Input_FIFO;
  p = NULL;
  idx = 0;

}

void render_X11::run(void)
{
  unsigned char*  rawFrame;
  unsigned char*  canvas;
  XImage*         ximage;

  display = XOpenDisplay(NULL);

  visual  = DefaultVisual(display, 0);

  window  = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, XSIZE, YSIZE, 1, 0, 0);

  canvas  = (unsigned char *)malloc(XSIZE*YSIZE*4);

  ximage = XCreateImage(display, visual, DefaultDepth(display,DefaultScreen(display)), ZPixmap, 0, (char *)canvas, XSIZE, YSIZE, 32, 0);

  XMapWindow(display, window);

  XFlush(display);

  _enableStream = true;

  while(_enableStream)
  {
    rawFrame = _videoStream->pop_last(); 
    if(rawFrame != NULL)
    {
      memcpy( (void *) canvas, (void *)rawFrame, XSIZE*YSIZE*4);

      XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, XSIZE, YSIZE);
      XFlush(display);

      free(rawFrame); // This will cost memory

      usleep(FRAMETIME); // Actually should correct for time used for rendering the image
    }
  }

}

void render_X11::stop(void)
{
  _enableStream = false;
}
