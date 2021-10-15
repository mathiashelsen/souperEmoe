#ifndef _C64_CONSTANTS_HPP
#define _C64_CONSTANTS_HPP

#define SCREEN_XSIZE                  320
#define SCREEN_YSIZE                  200

#define VIDEO_XSIZE                   320
#define VIDEO_YSIZE                   200
#define VIDEO_TOTAL_WIDTH             504
#define VIDEO_TOTAL_HEIGHT            312

#define COL_DEADTIME                  184
#define ROW_DEADTIME                  112

#define FRAMETIME                     20000
#define CYCLES_PER_FRAME              63*312

#define DEFAULT_KERNAL_ROM_BASE_ADDR  0xE000

#define DEFAULT_COLOR_RAM_BASE_ADDR   0xD800  // Absolute, same for CPU and VIC II
#define DEFAULT_CHAR_ROM_BASE_ADDR    0x0800  // From the VIC II stand point
#define DEFAULT_SCREEN_RAM_BASE_ADDR  0x0400  // From the VIC II stand point
#define DEFAULT_STACK_BASE_ADDR       0xFF

#define MEM_START_VICII_REG           0xD000
#define MEM_END_VICII_REG             0xD030

#define NMI_VECTOR                    0xFFFA
#define RST_VECTOR                    0xFFFC
#define IRQ_VECTOR                    0xFFFE
#define BRK_VECTOR                    0xFFFE

#endif
