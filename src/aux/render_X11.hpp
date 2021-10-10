#ifndef _RENDER_X11_HPP
#define _RENDER_X11_HPP

#include <mutex>
#include <iostream>
#include <chrono>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

#include "c64_constants.hpp"
#include "fifo.hpp"
#include "keystream.hpp"

class render_X11
{
  protected:
    Display*              display;
    Visual*               visual;
    Window                window;
    XEvent                event;
    fifo<unsigned char*>* _videoStream;
    bool                  _enableStream;
    int                   _OSR;
    keystream*            _keyStream;

    std::chrono::high_resolution_clock::time_point prevFrame;
    std::chrono::high_resolution_clock::time_point currFrame;
  public:
    render_X11(fifo<unsigned char *> *videoStream, int OSR);
    render_X11(fifo<unsigned char *> *videoStream, int OSR, keystream* keyStream);
    void run(void); // Function will fork a thread and keep looking at the FIFO
    void stop(void);
};

#endif
