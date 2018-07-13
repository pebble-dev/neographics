#pragma once
#include <pebble.h>

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                    Macros                                    |
|                                                                              |
|    General-purpose macros and system-specific things.                        |
|                                                                              |
`-----------------------------------------------------------------------------*/

#ifdef NGFX_IS_CORE
#if !defined NGFX_PREFERRED_free || \
    !defined NGFX_PREFERRED_calloc || \
    !defined NGFX_PREFERRED_malloc || \
    !defined NGFX_PREFERRED_resource_load
    #error "The macros for neographics are not defined!"
#endif
#else
#define NGFX_PREFERRED_free          (free)
#define NGFX_PREFERRED_calloc        (calloc)
#define NGFX_PREFERRED_malloc        (malloc)
#define NGFX_PREFERRED_resource_load (resource_load)
#endif

#define __BOUND_NUM(a, b, c) ((b) <= (a) ? (a) : ((b) >= (c) ? (c) : (b)))
#ifdef PBL_BW
#define __ARGB_TO_INTERNAL(a) ((((a) & 0b111111) == 0b111111) ? 0b11111111 :\
                               (((a) & 0b111111) == 0) ? 0b00000000 : 0b01010101)
#endif

#ifdef PBL_RECT
    #ifndef __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT
        #ifdef PBL_BW
            #define __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT (160 / 8)
        #else
            #define __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT (144)
        #endif
    #endif
    #define __SCREEN_WIDTH 144
    #define __SCREEN_HEIGHT 168
#else
    #define __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT 180
    #define __SCREEN_WIDTH 180
    #define __SCREEN_HEIGHT 180
#endif
