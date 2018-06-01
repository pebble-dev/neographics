#ifndef PBL_BW
#include "blit.h"
#include "../types.h"
#include "../context.h"

void n_graphics_blit_mem_copy(struct n_GContext *ctx, n_GBitmap *bitmap,
    n_GRect bounds, n_GPoint src_start) {
    uint8_t *fb_line = ctx->fbuf + bounds.origin.x +
        bounds.origin.y * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT;
    uint8_t *bm_line = bitmap->addr + bitmap->bounds.origin.x +
        src_start.y * bitmap->row_size_bytes;
    uint8_t* bm_stop = bitmap->addr + bitmap->bounds.origin.x +
        (bitmap->bounds.origin.y + bitmap->bounds.size.h) * bitmap->row_size_bytes;

    for (int y = 0; y < bounds.size.h; y++) {
        uint8_t *fb_pixels = fb_line;
        uint8_t *bm_pixels = bm_line;
        int w = bounds.size.w;

        if (src_start.x != bitmap->bounds.origin.x) {
            int offset = src_start.x - bitmap->bounds.origin.x;
            int rest_width = min(w, bitmap->bounds.size.w - offset);
            memcpy(fb_pixels, bm_pixels + offset, rest_width);
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
            int next_line = (src_start.y + y + 1) % bitmap->bounds.size.h;
            bm_line = bitmap->addr + bitmap->bounds.origin.x + next_line * bitmap->row_size_bytes;
        }
    }
}

void n_graphics_blit_blend(struct n_GContext *ctx, n_GBitmap *bitmap,
    n_GRect bounds, n_GPoint src_start) {

}

void n_graphics_blit_palette(struct n_GContext *ctx, n_GBitmap *bitmap,
    n_GRect bounds, n_GPoint src_start) {

}

#endif // !PBL_BW
