#pragma once
#include <pebble.h>
#include "../macros.h"
#include "../common.h"
#include "circle.h"
#include "line.h"

/*! \addtogroup rect
 *  Draw or fill rectangles with or without corner radii.
 *  @{
 */

/*!
 * Draws the border of a n_GRect with a stroke centered on the rect's outline.
 */
void n_graphics_draw_rect(n_GContext * ctx, n_GRect rect, uint16_t radius, n_GCornerMask mask);
/*!
 * Fills a n_GRect.
 */
void n_graphics_fill_rect(n_GContext * ctx, n_GRect rect, uint16_t radius, n_GCornerMask mask);

/*! \deprecated Use n_graphics_draw_rect() instead.
 *
 * Equivalent to graphics_draw_rect in Pebble's graphics implementation.
 * Always has a 1-px border.
 */
void n_graphics_draw_thin_rect(n_GContext * ctx, n_GRect rect);

/*! @}
 */
