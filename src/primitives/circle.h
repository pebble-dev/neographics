#pragma once
#include <pebble.h>
#include "../macros.h"
#include "../common.h"
#include "line.h"

/*! \addtogroup circle
 *  Functions for drawing or filling circles.
 *  @{
 */

/*!
 * Fills a circle.
 *
 * The circle is centered on a point, so it always has an odd diameter.
 */
void n_graphics_fill_circle(n_GContext * ctx, n_GPoint p, uint16_t radius);
/*!
 * Draws the outline of a circle.
 *
 * The circle is centered on a point, so it always has an odd diameter.
 */
void n_graphics_draw_circle(n_GContext * ctx, n_GPoint p, uint16_t radius);

/*!
 * Fills a circle within the given bounds.
 */
void n_graphics_fill_circle_bounded(n_GContext * ctx, n_GPoint p, uint16_t radius, int16_t minx, int16_t maxx, int16_t miny, int16_t maxy);
/*!
 * Draws the outline of a circle within the given bounds.
 */
void n_graphics_draw_circle_bounded(n_GContext * ctx, n_GPoint p, uint16_t radius, int16_t minx, int16_t maxx, int16_t miny, int16_t maxy);


/*!
 * Fills a quadrant of a circle.
 */
void n_graphics_prv_draw_quarter_circle_bounded(n_GContext * ctx, n_GPoint p,
    uint16_t radius, uint16_t width, int8_t x_dir, int8_t y_dir,
    int16_t minx, int16_t maxx, int16_t miny, int16_t maxy);
/*!
 * Draws the outline of a quadrant of a circle.
 */
void n_graphics_prv_fill_quarter_circle_bounded(n_GContext * ctx, n_GPoint p,
    uint16_t radius, int8_t x_dir, int8_t y_dir,
    int16_t minx, int16_t maxx, int16_t miny, int16_t maxy);

/*! @}
 */
