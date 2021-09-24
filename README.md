# superEmoe
A C64 Emulator in C++

# Basic operation:
The emulation will be operation accurate, but not cycle accurate. I don't think this will be much of an issue, because it only boils down to when an interrupt occurs (stop current operation or finish current operation)

```
 while( true ){
  t_start = now();
  for cycles in frame
  {
    processInterupts(); 
    CPU->doOperation();
    VICII->doOperation();
  }
  
  transferFrameToX11_Thread();
  t_end = now();
  sleep( frameTime - (t_end - t_start) );
 }
 ```
 
 * processInterrupts() will have to collect stuff from both the VICII (Raster Interupt) or the X11_Thread (keyboard interaction?)
 * CPU->doOperation() will perform an entire operation in an atomic fashion. Interrupt will happend after the current operation.
 * VICII->doOperation() will can stall the CPU, this will increase the "cycles" variable with "stolenCycles"
 * There will also be a memory object that will have to be connected. Interrupt will be put into a priority queue.
 * XLib will be used to render stuff.
