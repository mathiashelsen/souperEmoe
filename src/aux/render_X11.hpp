#ifndef _RENDER_X11_HPP
#define _RENDER_X11_HPP

#include <X11/Xlib.h>
#include "fifo.hpp"

class render_X11
{
  protected:
    Display *display;
    Visual *visual;
    Window window;
  public:
    render_X11(fifo<char> *Input_FIFO); //Will add an output FIFO later for keyboard/mouse input to the C64
    void run(void); // Function will fork a thread and keep looking at the FIFO
    void stop(void);
};

#endif
