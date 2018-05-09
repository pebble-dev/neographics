#pragma once
#include <pebble.h>
#include "types.h"
#include "macros.h"
#include "context.h"

/*-----------------------------------------------------------------------------.
|                                                                              |
|                          Common Graphics Routines                            |
|                                                                              |
|   The common graphics routines are fast pixel, row and column drawing        |
|   operations. All primitives should use these functions instead of           |
|   accessing the framebuffer.                                                 |
|                                                                              |
`-----------------------------------------------------------------------------*/

/*! \addtogroup low-level
 *  Low-level graphics routines. Not generally recommended for use, but
 *  available to anyone who needs some graphics speedup.
 *  @{
 */

/*!
 * Sets the color of a single pixel on the canvas.
 */
void n_graphics_set_pixel(n_GContext * ctx, n_GPoint p, n_GColor color);
/*!
* Sets the color of a single pixel on the canvas to the current fill color.
*/
void n_graphics_fill_pixel(n_GContext * ctx, n_GPoint p);
/*!
* Sets the color of a single pixel on the canvas to the current line color.
*/
void n_graphics_draw_pixel(n_GContext * ctx, n_GPoint p);

/*!
 * Draws a one-pixel column pretty quickly.
 */
void n_graphics_prv_draw_col(uint8_t * fb,
    int16_t x, int16_t top, int16_t bottom,
    int16_t minx, int16_t maxx, int16_t miny, int16_t maxy,
    uint8_t fill);

/*!
 * Draws a one-pixel row *very* quickly.
 */
void n_graphics_prv_draw_row(uint8_t * fb,
    int16_t y, int16_t left, int16_t right,
    int16_t minx, int16_t maxx, int16_t miny, int16_t maxy,
    uint8_t fill);

/*! @}
 */
