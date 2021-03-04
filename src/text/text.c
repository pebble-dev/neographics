#include "text.h"

// TODO possibly move from this to a "true" unicode subsystem

#define CODEPOINT_SPACE 32
#define __CODEPOINT_IGNORE_AT_LINE_END(a) ((a) == CODEPOINT_SPACE)
// Breaking after this character is a _good idea_
// (for example, breaking after spaces or hyphens is good)
// This is preferred to prebreaking. Hyphens after postbreakables are ignored.
// #define __CODEPOINT_GOOD_POSTBREAKABLE(a) (false)
#define __CODEPOINT_GOOD_POSTBREAKABLE(a) ((a) == CODEPOINT_SPACE)
// Breaking after this character is allowed
// (for example, you aren't allowed to break immediately before punctuation.)
#define __CODEPOINT_ALLOW_PREBREAKABLE(a) (!(\
        /* space            */ (a) == CODEPOINT_SPACE ||\
        /* exclamation mark */ (a) == 33 ||\
        /* double quote     */ (a) == 34 ||\
        /* comma            */ (a) == 44 ||\
        /* period           */ (a) == 46) \
    )

#define __CODEPOINT_NEEDS_HYPHEN_AFTER(a) (\
        /* numbers          */ ((a) >= 0x30 && (a) <= 0x39) ||\
        /* basic uppercase  */ ((a) >= 0x41 && (a) <= 0x5a) ||\
        /* basic lowercase  */ ((a) >= 0x61 && (a) <= 0x7a) ||\
        /* extended a and b */ ((a) >= 0x100 && (a) <= 0x24f) \
    )

static uint32_t n_graphics_codepoint_from_utf8(const char *text, int *bytes) {
    uint32_t codepoint = 0;
    int nbytes = 0;
    
    // We're following the 2003 UTF-8 definition:
    // 0b0xxxxxxx
    // 0b110xxxxx 0b10xxxxxx
    // 0b1110xxxx 0b10xxxxxx 0b10xxxxxx
    // 0b11110xxx 0b10xxxxxx 0b10xxxxxx 0b10xxxxxx
    
    if (text[0] & 0b10000000) {
        if ((text[0] & 0b11100000) == 0b11000000) {
            codepoint = ((text[0] &  0b11111) << 6)
                      +  (text[1] & 0b111111);
            nbytes = 2;
        } else if ((text[0] & 0b11110000) == 0b11100000) {
            codepoint = ((text[0] &   0b1111) << 12)
                      + ((text[1] & 0b111111) << 6)
                      +  (text[2] & 0b111111);
            nbytes = 3;
        } else if ((text[0] & 0b11111000) == 0b11110000) {
            codepoint = ((text[0] &    0b111) << 18)
                      + ((text[1] & 0b111111) << 12)
                      + ((text[2] & 0b111111) << 6)
                      +  (text[3] & 0b111111);
            nbytes = 4;
        } else {
            nbytes = 1;
        }
    } else {
        codepoint = text[0];
        nbytes = 1;
    }
    
    if (bytes) {
        *bytes = nbytes;
    }
    
    return codepoint;
}

static n_GPoint n_graphics_prv_draw_text_line(n_GContext * ctx, const char * text,
        uint32_t idx, uint32_t idx_end,
        n_GFont const font, n_GPoint text_origin) {
    while (idx < idx_end) {
        int nbytes;
        uint32_t codepoint = n_graphics_codepoint_from_utf8(text + idx, &nbytes);
        idx += nbytes;
        
        n_GGlyphInfo * glyph = n_graphics_font_get_glyph_info(font, codepoint);
        if (ctx) {
            n_graphics_font_draw_glyph(ctx, glyph, text_origin);
        }
        text_origin.x += glyph->advance;
    }
    return text_origin;
}

static n_GPoint n_graphics_prv_get_aligned_text_origin(const char * text, 
    uint32_t idx, uint32_t indx_end, const n_GRect box, const n_GPoint line_origin,
    n_GFont const font, uint8_t padding, const n_GTextAlignment alignment) {

    // Calculate origin of the character
    // where we are breaking the line.
    n_GPoint breakable_char_origin = box.origin;
    int nbytes;
    for (int i = idx; i < indx_end; i += nbytes)
        if (i == indx_end - 1 && __CODEPOINT_IGNORE_AT_LINE_END(*(text + i)))
            // Don't account for spaces at the end of lines
            continue;
        else {
            uint32_t codepoint = n_graphics_codepoint_from_utf8(text + i, &nbytes);
            // next_index += nbytes;
            breakable_char_origin.x += (n_graphics_font_get_glyph_info(font, codepoint))->advance;
        }

    breakable_char_origin.x += padding;

    switch (alignment) {
        case n_GTextAlignmentCenter:
            return n_GPoint(line_origin.x + (box.size.w - (breakable_char_origin.x - line_origin.x))/2,
                            line_origin.y);
        case n_GTextAlignmentRight:
            return n_GPoint(line_origin.x + box.size.w - (breakable_char_origin.x - line_origin.x),
                            line_origin.y);
            break;
        default:
            return line_origin;
    }
}

void n_graphics_draw_text_ex(
    n_GContext * ctx, const char * text, n_GFont const font, const n_GRect box,
    const n_GTextOverflowMode overflow_mode, const n_GTextAlignment alignment,
    n_GTextAttributes * text_attributes, n_GSize * outsize) {
    //TODO attributes

    // Rendering of text is done as follows:
    // - We store the index of the beginning of the line.
    // - We iterate over characters in the line.
    //    - Whenever an after-breakable character occurs, we make a note of it.
    //    - When the width of the line is exceeded, we actually render
    //      the line (up to the breakable character.)
    //    - We then use that character's index as the beginning
    //      of the next line.
    n_GPoint char_origin = box.origin, line_origin = box.origin;
    uint8_t line_height = n_graphics_font_get_line_height(font); /* Cache this, because it is expensive on RebbleOS. */
    uint32_t line_begin = 0, index = 0, next_index = 0;
    int32_t last_breakable_index = -1, last_renderable_index = -1,
            lenience = n_graphics_font_get_glyph_info(font, ' ')->advance;
    n_GGlyphInfo * hyphen = n_graphics_font_get_glyph_info(font, '-'),
                 * glyph = NULL;
    n_GPoint bbox = (n_GPoint) { .x = box.origin.x, .y = box.origin.y };
    n_GPoint line_endpt;

    uint32_t codepoint = 0, next_codepoint = 0, last_codepoint = 0,
        last_renderable_codepoint = 0, last_breakable_codepoint = 0;

    while (text[index] != '\0') {
        if (text[index] == '\n'
                && (char_origin.x + (__CODEPOINT_NEEDS_HYPHEN_AFTER(codepoint) ? hyphen->advance : 0)
                        <= box.origin.x + box.size.w)) {

            n_GPoint text_origin = n_graphics_prv_get_aligned_text_origin(
                text, line_begin, index, box, line_origin,
                font, 0, alignment);

            line_endpt = n_graphics_prv_draw_text_line(ctx, text,
                line_begin, index, font, text_origin);

            bbox.x = (bbox.x > line_endpt.x) ? bbox.x : line_endpt.x;
            char_origin.x = box.origin.x, char_origin.y += line_height;
            bbox.y = char_origin.y;
            last_breakable_index = last_renderable_index = -1;
            line_origin = char_origin;
            index = next_index = index + 1;
            line_begin = index;
            if (line_origin.y + line_height >= box.origin.y + box.size.h) {
                goto overflow;
            }
            continue;
        }

        int nbytes;
        next_codepoint = n_graphics_codepoint_from_utf8(text + index, &nbytes);
        next_index += nbytes;

        n_GGlyphInfo * next_glyph = n_graphics_font_get_glyph_info(font, next_codepoint);
        
        // Debugging:
        // n_graphics_context_set_text_color(ctx, n_GColorLightGray);
        // n_graphics_font_draw_glyph(ctx, next_glyph, char_origin);
        // n_graphics_context_set_text_color(ctx, n_GColorBlack);

        // We now know what codepoint the next character has.

        if (glyph) {
            if (__CODEPOINT_ALLOW_PREBREAKABLE(codepoint)) {
                if (char_origin.x +
                        (__CODEPOINT_NEEDS_HYPHEN_AFTER(last_codepoint)
                            ? hyphen->advance : 0)
                        <= box.origin.x + box.size.w) {
                    last_renderable_index = index;
                    last_renderable_codepoint = codepoint;
                }
            }
            if (__CODEPOINT_GOOD_POSTBREAKABLE(codepoint) &&
                    ((
                        (__CODEPOINT_IGNORE_AT_LINE_END(codepoint) &&
                        char_origin.x - glyph->advance <= box.origin.x + box.size.w) ||
                    char_origin.x <= box.origin.x + box.size.w))) {
                last_breakable_index = index;
                last_breakable_codepoint = codepoint;
            }
        }

        // Done processing the two available characters.

        index = next_index;
        last_codepoint = codepoint;
        codepoint = next_codepoint;
        glyph = next_glyph;
        char_origin.x += glyph->advance;

        bool did_text_line_exceed_width = (char_origin.x + (__CODEPOINT_NEEDS_HYPHEN_AFTER(codepoint) ? hyphen->advance : 0) - lenience > box.origin.x + box.size.w);

        if (did_text_line_exceed_width) {
            // This block runs if we have to print a line of text that needs to be broken up.
            // Single line text - this never runs.
            // Multi line text - this will print all but the last line.
            if (last_breakable_index > 0) {
                n_GPoint text_origin = n_graphics_prv_get_aligned_text_origin(
                    text, line_begin, last_breakable_index, box, line_origin,
                    font, 0, alignment);

                line_endpt = n_graphics_prv_draw_text_line(ctx, text, line_begin, last_breakable_index, font, text_origin);

                index = next_index = last_breakable_index;
                char_origin.x = box.origin.x, char_origin.y += line_height;
                line_begin = last_breakable_index;
                last_breakable_index = last_renderable_index = -1;
                line_origin = char_origin;
            } else if (last_renderable_index > 0) {
                // Break in the middle of a word, need to include hyphen
                n_GPoint text_origin = n_graphics_prv_get_aligned_text_origin(
                    text, line_begin, last_renderable_index, box, line_origin,
                    font, hyphen->advance, alignment);

                line_endpt = n_graphics_prv_draw_text_line(ctx, text, line_begin, last_renderable_index, font, text_origin);

                if (__CODEPOINT_NEEDS_HYPHEN_AFTER(last_renderable_codepoint) || true) { // TODO
                    if (ctx) {
                        n_graphics_font_draw_glyph(ctx, hyphen, line_endpt);
                        line_endpt.x += hyphen->advance;
                    }
                }
                index = next_index = last_renderable_index;
                char_origin.x = box.origin.x, char_origin.y += line_height;
                line_begin = last_renderable_index;
                last_breakable_index = last_renderable_index = -1;
                line_origin = char_origin;
            } else {
                if (ctx) {
                    n_graphics_font_draw_glyph(ctx, hyphen, line_origin);
                    line_endpt = line_origin;
                    line_endpt.x += hyphen->advance;
                }
                line_begin = next_index;
                char_origin.x = box.origin.x, char_origin.y += line_height;
                line_origin = char_origin;
            }
            bbox.x = (bbox.x > line_endpt.x) ? bbox.x : line_endpt.x;
            bbox.y = line_endpt.y + line_height;
            if (line_origin.y + line_height >= box.origin.y + box.size.h) {
                goto overflow;
            }
        }
        index += (0 * line_begin * last_breakable_codepoint);
    }
    if (index != line_begin) {
        // This block of code runs if we need to print the unbroken line of text.
        // With text that fits on one line, this what prints it.
        // With multiline text, this prints the last line.
        n_GPoint text_origin = n_graphics_prv_get_aligned_text_origin(
            text, line_begin, index, box, line_origin, font, 0, alignment);

        line_endpt = n_graphics_prv_draw_text_line(ctx, text,
            line_begin, index, font, text_origin);

        bbox.x = (bbox.x > line_endpt.x) ? bbox.x : line_endpt.x;
        bbox.y = line_endpt.y + line_height;
    }

overflow:
    if (outsize) {
        outsize->w = bbox.x - box.origin.x;
        outsize->h = bbox.y - box.origin.y;
    }

    return;
}

void n_graphics_draw_text(
    n_GContext * ctx, const char * text, n_GFont const font, const n_GRect box,
    const n_GTextOverflowMode overflow_mode, const n_GTextAlignment alignment,
    n_GTextAttributes * text_attributes) {

    n_graphics_draw_text_ex(ctx, text, font, box, overflow_mode, alignment,
        text_attributes, NULL);
}

n_GSize n_graphics_text_layout_get_content_size_with_attributes(
    const char * text, n_GFont const font, const n_GRect box,
    const n_GTextOverflowMode overflow_mode, const n_GTextAlignment alignment,
    n_GTextAttributes * text_attributes) {

    n_GSize outsz;

    n_graphics_draw_text_ex(NULL, text, font, box, overflow_mode, alignment,
        text_attributes, &outsz);

    return outsz;
}
