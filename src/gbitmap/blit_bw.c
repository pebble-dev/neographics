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

#endif // PBL_BW
