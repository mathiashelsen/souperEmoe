#include "render_X11.hpp"


render_X11::render_X11(fifo<unsigned char *> *Input_FIFO)
{
  _videoStream = Input_FIFO;

  display = XOpenDisplay(NULL);
  visual  = DefaultVisual(display, 0);
  window  = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, XSIZE, YSIZE, 1, 0, 0);

  _enableStream = true;
}

void render_X11::run(void)
{
  unsigned char*  rawFrame;
  XImage*         ximage;

  while(_enableStream)
  {
    rawFrame = _videoStream->pop_last(); 
    if(rawFrame != NULL)
    {
      ximage = XCreateImage(display, visual, DefaultDepth(display,DefaultScreen(display)), ZPixmap, 0, rawFrame, XSIZE, YSIZE, 32, 0);
      XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, XSIZE, YSIZE);
      delete rawFrame;
    }
    usleep(FRAMETIME); // Actually should correct for time used for rendering the image
  }

}

void render_X11::stop(void)
{
  _enableStream = false;
}
