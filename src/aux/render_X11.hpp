#ifndef _RENDER_X11_HPP
#define _RENDER_X11_HPP

#include <mutex>
#include <iostream>

#include <X11/Xlib.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

#include "c64_constants.hpp"
#include "fifo.hpp"

class render_X11
{
  protected:
    Display*              display;
    Visual*               visual;
    Window                window;
    fifo<unsigned char*>* _videoStream;
    bool                  _enableStream;
  public:
    render_X11(fifo<unsigned char *> *videoStream); //Will add an output FIFO later for keyboard/mouse input to the C64
    void run(void); // Function will fork a thread and keep looking at the FIFO
    void stop(void);
};

#endif
