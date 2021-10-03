#ifndef _ADC_VFCT_HPP
#define _ADC_VFCT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <thread>

#include <unistd.h>
#include <cstdio>

#include <iostream>

#include "../aux/fifo.hpp"
#include "../aux/render_X11.hpp"
#include "../aux/c64_constants.hpp"

#include "../core/Computer.hpp"

void adc_runtest(void);

#endif
