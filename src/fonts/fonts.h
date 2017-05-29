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
#include "../common.h"
#include "../context.h"

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                   Fonts                                      |
|                                                                              |
|                         Abstracts away font magicks.                         |
|                                                                              |
`-----------------------------------------------------------------------------*/

/*! \addtogroup fonts
 *  Get glyph information.
 *  @{
 */

/*!
 * Stores basic information about a font.
 */
typedef struct n_GFontInfo {
    uint8_t version;
    uint8_t line_height;
    uint16_t glyph_amount;
    uint16_t wildcard_codepoint;
    // v2+
    uint8_t hash_table_size;
    uint8_t codepoint_bytes;
    // v3+
    uint8_t fontinfo_size;
    uint8_t features;
} __attribute__((__packed__)) n_GFontInfo;

/*!
 * An entry in the hash table, which stores a hash (computed by: codepoint
 * modulo hash table length) and gives the offset table offset for that value.
 */
typedef struct n_GFontHashTableEntry {
    uint8_t hash_value;
    uint8_t offset_table_size;
    uint16_t offset_table_offset;
} __attribute__((__packed__)) n_GFontHashTableEntry;

/*!
 * Length of the n_GFontInfo for font version 1
 */
#define __FONT_INFO_V1_LENGTH 6
/*!
 * Length of the n_GFontInfo for font version 2
 */
#define __FONT_INFO_V2_LENGTH 8

/*!
 * Bitmasks which determine whether certain features are
 * available in a font.
 */
typedef enum {
    n_GFontFeature2ByteGlyphOffset = 0b1,
    n_GFontFeatureRLE4Encoding = 0b10,
} n_GFontFeatures;

/*!
 * Metadata and data of a glyph.
 */
typedef struct n_GGlyphInfo {
    uint8_t width;
    uint8_t height;
    int8_t left_offset;
    int8_t top_offset;
    int8_t advance;
    uint8_t data[];
} __attribute__((__packed__)) n_GGlyphInfo;

/*!
 * Alias for user-facing API.
 */
typedef n_GFontInfo * n_GFont;

/*!
 * Get the line height of a given font.
 */
uint8_t n_graphics_font_get_line_height(n_GFont font);

/*!
 * Get glyph information for a charcode from a font (tofu if not available)
 */
n_GGlyphInfo * n_graphics_font_get_glyph_info(n_GFont font, uint32_t charcode);

/*!
 * Draw a glyph.
 */
void n_graphics_font_draw_glyph(n_GContext * ctx, n_GGlyphInfo * glyph, n_GPoint p);

/*! @}
 */
