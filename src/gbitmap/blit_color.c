#ifndef PBL_BW
#include "blit.h"
#include "types.h"
#include "context.h"

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

void n_graphics_blit_mem_copy(struct n_GContext *ctx, const n_GBitmap *bitmap,
    n_GRect bounds, n_GPoint src_offset) {
    uint8_t *fb_line = ctx->fbuf + bounds.origin.x +
        bounds.origin.y * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT;
    uint8_t *bm_line = bitmap->addr + bitmap->bounds.origin.x +
        (bitmap->bounds.origin.y + src_offset.y) * bitmap->row_size_bytes;
    uint8_t* bm_stop = bitmap->addr + bitmap->bounds.origin.x +
        (bitmap->bounds.origin.y + bitmap->bounds.size.h) * bitmap->row_size_bytes;

    for (int y = 0; y < bounds.size.h; y++) {
        uint8_t *fb_pixels = fb_line;
        uint8_t *bm_pixels = bm_line;
        int w = bounds.size.w;

        if (src_offset.x > 0) {
            int rest_width = min(w, bitmap->bounds.size.w - src_offset.x);
            memcpy(fb_pixels, bm_pixels + src_offset.x, rest_width);
            fb_pixels += rest_width;
            w -= rest_width;
        }
        while (w >= bitmap->bounds.size.w) {
            memcpy(fb_pixels, bm_pixels, bitmap->bounds.size.w);
            fb_pixels += bitmap->bounds.size.w;
            w -= bitmap->bounds.size.w;
        }
        if (w > 0)
            memcpy(fb_pixels, bm_pixels, w);

        fb_line += __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT;
        bm_line += bitmap->row_size_bytes;
        if (bm_line >= bm_stop) {
            int next_line = bitmap->bounds.origin.y + (src_offset.y + y + 1) % bitmap->bounds.size.h;
            bm_line = bitmap->addr + bitmap->bounds.origin.x + next_line * bitmap->row_size_bytes;
        }
    }
}

void n_graphics_blit_blend(struct n_GContext *ctx, const n_GBitmap *bitmap,
    n_GRect bounds, n_GPoint src_offset) {
    n_GColor *fb_line = (n_GColor*)ctx->fbuf + bounds.origin.x +
        bounds.origin.y * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT;
    n_GColor* bm_first_line = (n_GColor*)bitmap->addr + bitmap->bounds.origin.x +
        bitmap->bounds.origin.y * bitmap->row_size_bytes;

    for (int y = 0; y < bounds.size.h; y++) {
        n_GColor *fb_pixel = fb_line;
        n_GColor *bm_line = bm_first_line +
            ((src_offset.y + y) % bitmap->bounds.size.h) * bitmap->row_size_bytes;

        for (int x = 0; x < bounds.size.w; x++) {
            n_GColor bm_pixel = bm_line[(src_offset.x + x) % bitmap->bounds.size.w];
            *fb_pixel = n_gcolor_blend(*fb_pixel, bm_pixel);
            fb_pixel++;
        }

        fb_line += __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT;
    }
}

void n_graphics_blit_palette(struct n_GContext *ctx, const n_GBitmap *bitmap,
    n_GRect bounds, n_GPoint src_offset) {
    static const uint8_t bw_palettes[][2] = { // uint8 is easier to declare for constants
        [n_GCompOpAssign] =         { n_GColorBlackARGB8, n_GColorWhiteARGB8 },
        [n_GCompOpAssignInverted] = { n_GColorWhiteARGB8, n_GColorBlackARGB8},
        [n_GCompOpAnd] =            { n_GColorBlackARGB8, n_GColorClearARGB8 },
        [n_GCompOpOr] =             { n_GColorClearARGB8, n_GColorWhiteARGB8 },
        [n_GCompOpClear] =          { n_GColorClearARGB8, n_GColorWhiteARGB8 },
        [n_GCompOpSet] =            { n_GColorWhiteARGB8, n_GColorClearARGB8}
    };

    // Determine actual palette to use and stuff like bit length/mask 
    uint8_t bits_per_pixel = n_gbitmapformat_get_bits_per_pixel(bitmap->format);
    uint8_t index_mask = (1 << bits_per_pixel) - 1;
    uint8_t pixels_per_byte = 8 / bits_per_pixel;
    int offset_in_byte = pixels_per_byte - 1;
    const n_GColor* palette = bitmap->palette;
    if (bitmap->format == n_GBitmapFormat1Bit) {
        palette = (const n_GColor*)bw_palettes[ctx->comp_op];
        offset_in_byte = 0;
    }

    // Blit the bitmap
    n_GColor *fb_line = (n_GColor*)ctx->fbuf + bounds.origin.x +
        bounds.origin.y * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT;
    uint8_t* bm_first_line = bitmap->addr +
        bitmap->bounds.origin.y * bitmap->row_size_bytes;

    for (int y = 0; y < bounds.size.h; y++) {
        n_GColor *fb_pixel = fb_line;
        uint8_t *bm_line = bm_first_line +
            ((src_offset.y + y) % bitmap->bounds.size.h) * bitmap->row_size_bytes;

        for (int x = 0; x < bounds.size.w; x++) {
            int src_x = bitmap->bounds.origin.x + (src_offset.x + x) % bitmap->bounds.size.w;
            uint8_t src_pixel_byte = bm_line[src_x / pixels_per_byte];
            int src_pixel_bit = abs(offset_in_byte - src_x % pixels_per_byte) * bits_per_pixel;
            int src_color_index = (src_pixel_byte >> src_pixel_bit) & index_mask;
            n_GColor src_color = palette[src_color_index];

            if (ctx->comp_op == n_GCompOpAssign)
                *fb_pixel = src_color;
            else
                *fb_pixel = n_gcolor_blend(*fb_pixel, src_color);
            fb_pixel++;
        }

        fb_line += __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT;
    }
}

#endif // !PBL_BW
