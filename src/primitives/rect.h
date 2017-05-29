/*\
|*|
|*|   Neographics: a tiny graphics library.
|*|   Copyright (C) 2016 Johannes Neubrand <johannes_n@icloud.com>
|*|
|*|   This program is free software; you can redistribute it and/or
|*|   modify it under the terms of the GNU General Public License
|*|   as published by the Free Software Foundation; either version 2
|*|   of the License, or (at your option) any later version.
|*|
|*|   This program is distributed in the hope that it will be useful,
|*|   but WITHOUT ANY WARRANTY; without even the implied warranty of
|*|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|*|   GNU General Public License for more details.
|*|
|*|   You should have received a copy of the GNU General Public License
|*|   along with this program; if not, write to the Free Software
|*|   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
|*|
\*/

#pragma once
#include <pebble.h>
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
