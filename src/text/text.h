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
#include "../types.h"
#include "../macros.h"
#include "../context.h"
#include "../fonts/fonts.h"

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                    Text                                      |
|                                                                              |
`-----------------------------------------------------------------------------*/

/*! \addtogroup text
 *  Renders text.
 *  @{
 */

/*!
 * Determines how line wrapping is handled.
 */
typedef enum n_GTextOverflowMode {
    n_GTextOverflowModeWordWrap = 0,
        // "Normal" filling mode. Respects \n, cuts off at end.
    n_GTextOverflowModeTrailingEllipsis,
        // Like WordWrap, but adds a trailing ellipsis at the last renderable
        // position if there is additional text.
    n_GTextOverflowModeFill,
        // Renders \n as a space. Has a trailing ellipsis. Is buggy.
} n_GTextOverflowMode;

/*!
 * Determines how text is aligned within a line.
 */
typedef enum n_GTextAlignment {
    n_GTextAlignmentLeft = 0,
    n_GTextAlignmentCenter,
    n_GTextAlignmentRight,
} n_GTextAlignment;

/*!
 * Represents data relevant to rendering text on non-rectangular screens or with
 * paging.
 */
typedef struct n_GTextAttributes {
    bool use_screen_text_flow;
    uint8_t inset;
    bool use_paging;
    n_GRect pageable_area;
    n_GPoint text_origin;
} __attribute__((__packed__)) n_GTextAttributes;

/*!
 * Creates a n_GTextAttributes struct.
 */
n_GTextAttributes * n_graphics_text_attributes_create(void);

/*!
 * Destroys a n_GTextAttributes struct.
 */
void n_graphics_text_attributes_destroy(n_GTextAttributes * text_attributes);

/*!
 * Enables screen text flow in a n_GTextAttributes struct.
 */
void n_graphics_text_attributes_enable_screen_text_flow(
    n_GTextAttributes * text_attributes, uint8_t inset);

/*!
 * Resets paging behavior of a given n_GTextAttributes struct to default.
 */
void n_graphics_text_attributes_restore_default_paging(
    n_GTextAttributes * text_attributes);

/*!
 * Enables paging for a given n_GTextAttributes struct.
 */
void n_graphics_text_attributes_enable_paging(
    n_GTextAttributes * text_attributes, n_GPoint moduloed_origin, n_GRect pageable_area);

/*!
 * Draw text with given paramters.
 */
void n_graphics_draw_text(
    n_GContext * ctx, const char * text, n_GFont const font, const n_GRect box,
    const n_GTextOverflowMode overflow_mode, const n_GTextAlignment alignment,
    n_GTextAttributes * text_attributes);

/*!
 * Query dimensions of a given text block.
 */
n_GSize n_graphics_text_layout_get_content_size_with_attributes(
    const char * text, n_GFont const font, const n_GRect box,
    const n_GTextOverflowMode overflow_mode, const n_GTextAlignment alignment,
    n_GTextAttributes * text_attributes);

/*!
 * Macro for reverse-compatibility for attributeless content size querying.
 */
#define n_graphics_text_layout_get_content_size(a, b, c, d)\
        (n_graphics_text_layout_get_content_size_with_attributes((a), (b), (c), (d), NULL));

/*! @}
 */
