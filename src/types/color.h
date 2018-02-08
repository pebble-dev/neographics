#pragma once
#ifdef NGFX_IS_CORE
#include <stdint.h>
#else
#include <pebble.h>
#endif
#include "colors.h"

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                   Color                                      |
|                                                                              |
`-----------------------------------------------------------------------------*/

/*! \addtogroup Types
 *  @{
 *  \addtogroup Color
 *  Color-handling code.
 *  @{
 */

/*!
 * Internal representation of colors: now with revolutionary 2-bit-per-channel
 * technology!
 */
typedef union n_GColor8 {
    struct {
        // little endian, so reverse field order (within byte)
        uint8_t b:2;
        uint8_t g:2;
        uint8_t r:2;
        uint8_t a:2;
    };
    uint8_t argb;
} n_GColor8;

typedef n_GColor8 n_GColor;

/*!
 * Check for the equality of two n_GColor(s).
 */
inline bool n_gcolor_equal(n_GColor8 a, n_GColor8 b) {
   return a.argb == b.argb;
}

/*!
 * Get the ideal text n_GColor over a given background.
 */
inline n_GColor8 n_gcolor_legible_over(n_GColor8 color) {
   return (color.r + color.g + color.b > 6) ? n_GColorWhite : n_GColorBlack;
}

/*!
 * Convenience macro to create the closest n_GColor to 4 8-bit values.
 */
#define n_GColorFromRGBA(_r, _g, _b, _a) \
    ((n_GColor8) {{.a = ((_a) >> 6) & 0b11, .r = ((_r) >> 6) & 0b11,\
                   .g = ((_g) >> 6) & 0b11, .b = ((_b) >> 6) & 0b11}})

/*!
 * Convenience macro to create the closest n_GColor to 3 8-bit values.
 */
#define n_GColorFromRGB(_r, _g, _b) \
    ((n_GColor8) {{.a =               0b11, .r = ((_r) >> 6) & 0b11,\
                   .g = ((_g) >> 6) & 0b11, .b = ((_b) >> 6) & 0b11}})

/*!
 * Convenience macro to create the closest n_GColor to a hex value (eg 0xFF7700).
 */
#define n_GColorFromHEX(_h) n_GColorFromRGB(((_h) >> 16) & 0b11111111, \
                                            ((_h) >>  8) & 0b11111111, \
                                             (_h)        & 0b11111111)

/*! @}
 *  @}
 */
