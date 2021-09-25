#include "render_X11.hpp"


render_X11::render_X11(fifo<unsigned char *> *Input_FIFO)
{
  _videoStream = Input_FIFO;

}

void render_X11::run(void)
{
  display = XOpenDisplay(NULL);
  if(display == NULL)
    std::cout << "Could NOT create display" << std::endl;
  else
    std::cout << "Could create display" << std::endl;

  visual  = DefaultVisual(display, 0);
  if(visual == NULL)
    std::cout << "Could NOT create visual" << std::endl;
  else
    std::cout << "Could create visual" << std::endl;

  window  = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, XSIZE, YSIZE, 1, 0, 0);

  XMapWindow(display, window);
  _enableStream = true;
  std::cout << "Window created... " << std::endl;

  XFlush(display);

  unsigned char*  rawFrame;
  XImage*         ximage;

  while(_enableStream)
  {
    std::cout << "Waiting for data... " << std::endl;
    rawFrame = _videoStream->pop_last(); 
    if(rawFrame != NULL)
    {
      std::cout << "Got some data... " << std::endl;
      ximage = XCreateImage(display, visual, DefaultDepth(display,DefaultScreen(display)), ZPixmap, 0, rawFrame, XSIZE, YSIZE, 32, 0);
      int retVal = XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, XSIZE, YSIZE);
      if(retVal == 0)
        std::cout << "XPutImage was successfull" << std::endl;
      else
        std::cout << "XPutImage had issues" << std::endl;

      //free(rawFrame);
      XFlush(display);
    }
    usleep(FRAMETIME); // Actually should correct for time used for rendering the image
  }

}

void render_X11::stop(void)
{
  _enableStream = false;
}
