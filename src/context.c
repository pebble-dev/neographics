#include "context.h"

// TODO optimization: calculate bytefill when color is set.

void n_graphics_context_set_stroke_color(n_GContext * ctx, n_GColor color) {
    ctx->stroke_color = color;
}

void n_graphics_context_set_fill_color(n_GContext * ctx, n_GColor color) {
    ctx->fill_color = color;
}

void n_graphics_context_set_text_color(n_GContext * ctx, n_GColor color) {
    ctx->text_color = color;
}

// void n_graphics_context_set_compositing_mode(n_GContext * ctx, ...) {
// }

void n_graphics_context_set_antialiased(n_GContext * ctx, bool enable) {
    ctx->antialias = enable;
}

void n_graphics_context_set_stroke_width(n_GContext * ctx, uint16_t width) {
    if (width == 0)
        return;
    ctx->stroke_width = ((width + 2) & ~(1)) - 1;
}

void n_graphics_context_set_stroke_caps(n_GContext * ctx, bool stroke_caps) {
    ctx->stroke_caps = stroke_caps;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void n_graphics_context_begin(n_GContext * ctx) {
    if (ctx->underlying_context) {
        ctx->bitmap = graphics_capture_frame_buffer(ctx->underlying_context);
        ctx->fbuf = gbitmap_get_data(ctx->bitmap);
    }
}

void n_graphics_context_end(n_GContext * ctx) {
    if (ctx->underlying_context) {
        graphics_release_frame_buffer(ctx->underlying_context, ctx->bitmap);
        ctx->bitmap = NULL;
        ctx->fbuf = NULL;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

GBitmap * n_graphics_capture_frame_buffer(n_GContext * ctx) {
#ifndef NGFX_IS_CORE
    return graphics_capture_frame_buffer(ctx->underlying_context);
#else
    return graphics_capture_frame_buffer(ctx);
#endif
}

GBitmap * n_graphics_capture_frame_buffer_format(n_GContext * ctx, GBitmapFormat format) {
#ifndef NGFX_IS_CORE
    return graphics_capture_frame_buffer_format(ctx->underlying_context, format);
#else
    return graphics_capture_frame_buffer_format(ctx, format);
#endif
}

bool n_graphics_release_frame_buffer(n_GContext * ctx, GBitmap * bitmap) {
#ifndef NGFX_IS_CORE
    return graphics_release_frame_buffer(ctx->underlying_context, bitmap);
#else
    return graphics_release_frame_buffer(ctx, bitmap);
#endif
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static n_GContext * n_graphics_context_create() {
    n_GContext * out = NGFX_PREFERRED_calloc(1, sizeof(n_GContext));
#ifndef NGFX_IS_CORE
    out->underlying_context = NULL;
#endif
    n_graphics_context_set_stroke_color(out, (n_GColor) {.argb = 0b11000000});
    n_graphics_context_set_fill_color(out, (n_GColor) {.argb = 0b11111111});
    n_graphics_context_set_text_color(out, (n_GColor) {.argb = 0b11000000});
    // n_graphics_context_set_compositing_mode(out, )
    n_graphics_context_set_stroke_caps(out, true);
    n_graphics_context_set_antialiased(out, true);
    n_graphics_context_set_stroke_width(out, 1);
    return out;
}

n_GContext * n_graphics_context_from_buffer(uint8_t * buf) {
    n_GContext * out = n_graphics_context_create();
    out->fbuf = buf;
    return out;
}

n_GContext * n_graphics_context_from_graphics_context(GContext * ctx) {
    n_GContext * out = n_graphics_context_create();
    out->underlying_context = ctx;
    return out;
}

void n_graphics_context_destroy(n_GContext * ctx) {
    NGFX_PREFERRED_free(ctx);
}
