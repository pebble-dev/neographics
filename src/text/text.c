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

#include "text.h"

void n_graphics_draw_text(
    n_GContext * ctx, const char * text, n_GFont const font, const n_GRect box,
    const n_GTextOverflowMode overflow_mode, const n_GTextAlignment alignment,
    n_GTextAttributes * text_attributes) {
    //TODO overflowing (incl. wrapping)
    //TODO alignment
    //TODO attributes
    n_GPoint char_begin = box.origin;
    uint32_t i = 0;
    while (text[i] != '\0') {
        // We're following the 2003 UTF-8 definition:
        // 0b0xxxxxxx
        // 0b110xxxxx 0b10xxxxxx
        // 0b1110xxxx 0b10xxxxxx 0b10xxxxxx
        // 0b11110xxx 0b10xxxxxx 0b10xxxxxx 0b10xxxxxx
        uint32_t codepoint = 0;
        n_GGlyphInfo * glyph;
        if (text[i] == '\n') {
            char_begin.x = box.origin.x;
            char_begin.y += n_graphics_font_get_line_height(font);
            i++;
            continue;
        } else if (text[i] & 0b10000000) { // begin of multibyte character
            if ((text[i] & 0b11100000) == 0b11000000) {
                codepoint = ((text[i  ] &  0b11111) << 6)
                          +  (text[i+1] & 0b111111);
                i += 2;
            } else if ((text[i] & 0b11110000) == 0b11100000) {
                codepoint = ((text[i  ] &   0b1111) << 12)
                          + ((text[i+1] & 0b111111) << 6)
                          +  (text[i+2] & 0b111111);
                i += 3;
            } else if ((text[i] & 0b11111000) == 0b11110000) {
                codepoint = ((text[i  ] &    0b111) << 18)
                          + ((text[i+1] & 0b111111) << 12)
                          + ((text[i+2] & 0b111111) << 6)
                          +  (text[i+3] & 0b111111);
                i += 4;
            } else {
                i += 1;
            }
            glyph = n_graphics_font_get_glyph_info(font, codepoint);
        } else {
            glyph = n_graphics_font_get_glyph_info(font, text[i]);
            i += 1;
        }
        n_graphics_font_draw_glyph(ctx, glyph, char_begin);
        char_begin.x += glyph->advance;
    }
}
