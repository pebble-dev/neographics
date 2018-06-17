#ifdef PBL_BW
#include "blit.h"
#include "../types.h"
#include "../context.h"

void n_graphics_blit_comp(struct n_GContext *ctx, const n_GBitmap *bitmap,
    n_GRect bounds, n_GPoint src_offset) {
    for (int y = 0; y < bounds.size.h; y++) {
        for (int x = 0; x < bounds.size.w; x++) {
            int dst_x = bounds.origin.x + x;
            uint8_t* dst_byte = &ctx->fbuf[
                dst_x / 8 + (bounds.origin.y + y) * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT
            ];
            int dst_bit_shift = dst_x % 8;
            int dst_bit = (*dst_byte >> dst_bit_shift) & 1;

            int src_x = bitmap->bounds.origin.x + (src_offset.x + x) % bitmap->bounds.size.w;
            int src_y = bitmap->bounds.origin.y + (src_offset.y + y) % bitmap->bounds.size.h;
            int src_byte = bitmap->addr[src_y * bitmap->row_size_bytes + src_x / 8];
            int src_bit = (src_byte >> (src_x % 8)) & 1;

            int result_bit;
            switch (ctx->comp_op) {
                case n_GCompOpAssign:           result_bit = src_bit; break;
                case n_GCompOpAssignInverted:   result_bit = ~src_bit; break;
                case n_GCompOpAnd:              result_bit = dst_bit & src_bit; break;
                case n_GCompOpOr:               result_bit = dst_bit | src_bit; break;
                case n_GCompOpClear:            result_bit = dst_bit & ~src_bit; break;
                case n_GCompOpSet:              result_bit = dst_bit | ~src_bit; break;
            }
            *dst_byte = (*dst_byte & ~(1 << dst_bit_shift)) | ((result_bit & 1) << dst_bit_shift);
        }
    }
}

void n_graphics_blit_alpha(struct n_GContext *ctx, const n_GBitmap *bitmap,
    n_GRect bounds, n_GPoint src_offset) {
    
    // Convert the palette from color to black/white alpha/non-alpha 
    uint16_t color_palette = 0;
    uint16_t alpha_palette = 0xffff;
    for (int i = 0; i < bitmap->palette_size; i++) {
        n_GColor c = bitmap->palette[i];
        color_palette |= (c.r + c.g + c.b > 6) << i;
        if (ctx->comp_op == n_GCompOpSet && c.a < 2)
            alpha_palette &= ~(1 << i);
    }
    
    uint8_t bits_per_pixel = n_gbitmapformat_get_bits_per_pixel(bitmap->format);
    uint8_t index_mask = (1 << bits_per_pixel) - 1;
    uint8_t pixels_per_byte = 8 / bits_per_pixel;

    for (int y = 0; y < bounds.size.h; y++) {
        for (int x = 0; x < bounds.size.w; x++) {
            int dst_x = bounds.origin.x + x;
            uint8_t* dst_byte = &ctx->fbuf[
                dst_x / 8 + (bounds.origin.y + y) * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT
            ];
            int dst_bit_shift = dst_x % 8;
            int dst_bit = (*dst_byte >> dst_bit_shift) & 1;

            int src_x = bitmap->bounds.origin.x + (src_offset.x + x) % bitmap->bounds.size.w;
            int src_y = bitmap->bounds.origin.y + (src_offset.y + y) % bitmap->bounds.size.h;
            int src_byte = bitmap->addr[src_y * bitmap->row_size_bytes + src_x / pixels_per_byte];
            int src_bit = (src_x % pixels_per_byte) * bits_per_pixel;
            int src_index = (src_byte >> src_bit) & index_mask;
            int src_color = color_palette >> src_index;
            int src_alpha = alpha_palette >> src_index;

            int result_bit = (src_alpha & src_color) | (~src_alpha & dst_bit);

            *dst_byte = (*dst_byte & ~(1 << dst_bit_shift)) | ((result_bit & 1) << dst_bit_shift);
        }
    }
}

#endif // PBL_BW
