#include "render_X11.hpp"

// For debug
unsigned char *p;
volatile int idx;

XImage *CreateTrueColorImage2(Display *display, Visual *visual, int width, int height)
{
    int i, j;
    unsigned char *image32=(unsigned char *)malloc(width*height*4);
    unsigned char *p=image32;
    if(idx == 200)
      idx = 10;
    else
      idx += 10;

    for(i=0; i<width; i++)
    {
        for(j=0; j<height; j++)
        {
            if((i<256)&&(j<256))
            {
                *p++=idx; // blue
                *p++=idx; // green
                *p++=idx; // red
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
    return XCreateImage(display, visual, DefaultDepth(display,DefaultScreen(display)), ZPixmap, 0, image32, width, height, 32, 0);
}

render_X11::render_X11(fifo<unsigned char *> *Input_FIFO)
{
  _videoStream = Input_FIFO;
  p = NULL;
  idx = 0;

}

void render_X11::run(void)
{
  display = XOpenDisplay(NULL);

  visual  = DefaultVisual(display, 0);

  window  = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, XSIZE, YSIZE, 1, 0, 0);

  XMapWindow(display, window);
  _enableStream = true;

  XFlush(display);

  unsigned char*  rawFrame;
  unsigned char*  rcvdFrame;
  XImage*         ximage;


  XFlush(display);

  ximage = CreateTrueColorImage2(display, visual, XSIZE, YSIZE);
  int retVal = XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, XSIZE, YSIZE);

  XMapWindow(display, window);
  //free(rawFrame);
  XFlush(display);

  while(_enableStream)
  {
    rawFrame = _videoStream->pop_last(); 
    //if(rawFrame != NULL)
    //{
    //  rcvdFrame = rawFrame;
    //}
    ximage = CreateTrueColorImage2(display, visual, XSIZE, YSIZE);
    retVal = XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, XSIZE, YSIZE);
    usleep(FRAMETIME); // Actually should correct for time used for rendering the image
  }

}

void render_X11::stop(void)
{
  _enableStream = false;
}
