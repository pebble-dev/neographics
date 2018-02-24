#include "gbitmap.h"
#include "macros.h"

static uint8_t n_prv_palette_size[] = {
    0, /* n_GBitmapFormat1Bit */
    0, /* n_GBitmapFormat8Bit */
    2, /* n_GBitmapFormat1BitPalette */
    4, /* n_GBitmapFormat2BitPalette */
    8, /* n_GBitmapFormat4BitPalette */
    0, /* n_GBitmapFormat8BitCircular */
};

uint16_t n_gbitmap_get_bytes_per_row(const n_GBitmap *bitmap) {
    return bitmap->row_size_bytes;
}

n_GBitmapFormat n_gbitmap_get_format(const n_GBitmap *bitmap) {
    return bitmap->format;
}

uint8_t* n_gbitmap_get_data(const n_GBitmap *bitmap) {
    return bitmap->addr;
}

void n_gbitmap_set_data(n_GBitmap *bitmap, uint8_t *data, n_GBitmapFormat format, uint16_t row_size_bytes, bool free_on_destroy) {
    if (bitmap->free_data_on_destroy && bitmap->addr != NULL) {
        NGFX_PREFERRED_free(bitmap->addr);
    }
    bitmap->addr = data;
    bitmap->format = format;
    bitmap->row_size_bytes = row_size_bytes;
    bitmap->free_data_on_destroy = free_on_destroy;
}

n_GRect n_gbitmap_get_bounds(const n_GBitmap *bitmap) {
    return bitmap->bounds;
}

void n_gbitmap_set_bounds(n_GBitmap *bitmap, n_GRect bounds) {
    bitmap->bounds = bounds;
}

n_GColor* n_gbitmap_get_palette(const n_GBitmap *bitmap) {
    return bitmap->palette;
}

void n_gbitmap_set_palette(n_GBitmap *bitmap, n_GColor *palette, bool free_on_destroy) {
    if (bitmap->free_palette_on_destroy && bitmap->palette != NULL) {
        NGFX_PREFERRED_free(bitmap->palette);
    }
    bitmap->palette = palette;
    bitmap->free_palette_on_destroy = free_on_destroy;
}

n_GBitmap* n_gbitmap_create_with_data(const uint8_t *data) {
    return NULL; //TODO: This should load a PBI
}

n_GBitmap* n_gbitmap_create_as_sub_bitmap(const n_GBitmap *base_bitmap, n_GRect sub_rect) {
    n_grect_clip(&sub_rect, &base_bitmap->bounds);

    n_GBitmap *sub_bitmap = (n_GBitmap*)NGFX_PREFERRED_malloc(sizeof(n_GBitmap));
    if (sub_bitmap == NULL)
        return NULL;
    memcpy(sub_bitmap, base_bitmap, sizeof(n_GBitmap));
    sub_bitmap->free_palette_on_destroy = false;
    sub_bitmap->free_data_on_destroy = false;
    sub_bitmap->bounds = sub_rect;

    return sub_bitmap;
}

static n_GBitmap* n_prv_gbitmap_create_with_palette(n_GSize size, n_GBitmapFormat format, n_GColor *palette, bool free_on_destroy) {
    n_GBitmap* bitmap = (n_GBitmap*)NGFX_PREFERRED_malloc(sizeof(n_GBitmap));
    if (bitmap == NULL) {
        return NULL;
    }
    
    switch (format) {
        case n_GBitmapFormat1Bit:
        case n_GBitmapFormat1BitPalette:
            bitmap->row_size_bytes = (size.w + 7) / 8;
            break;
        case n_GBitmapFormat2BitPalette:
            bitmap->row_size_bytes = (size.w * 2 + 7) / 8;
            break;
        case n_GBitmapFormat4BitPalette:
            bitmap->row_size_bytes = (size.w * 4 + 7) / 8;
            break;
        case n_GBitmapFormat8Bit:
        case n_GBitmapFormat8BitCircular:
        default:
            bitmap->row_size_bytes = size.w;
            break;
    }

    bitmap->addr = (uint8_t*)NGFX_PREFERRED_malloc(size.h * bitmap->row_size_bytes);
    if (bitmap->addr == NULL) {
        NGFX_PREFERRED_free(bitmap);
        return NULL;
    }
    bitmap->raw_bitmap_size = size;
    bitmap->palette = palette;
    bitmap->palette_size = n_prv_palette_size[format];
    bitmap->free_palette_on_destroy = free_on_destroy;
    bitmap->free_data_on_destroy = true;
    bitmap->bounds = (n_GRect) { .origin = { 0, 0 }, .size = size };
    bitmap->format = format;

    return bitmap;
}

static n_GBitmap* n_prv_gbitmap_create(n_GSize size, n_GBitmapFormat format) {
    uint8_t palette_size = n_prv_palette_size[format];
    n_GColor* palette = (n_GColor*)NGFX_PREFERRED_calloc(palette_size, sizeof(n_GColor));
    memset(palette, 0, palette_size * sizeof(n_GColor));

    n_GBitmap* bitmap = n_prv_gbitmap_create_with_palette(size, format, palette, true);
    if (bitmap == NULL) {
        NGFX_PREFERRED_free(palette);
    }
    return bitmap;
}

n_GBitmap* n_gbitmap_create_blank(n_GSize size, n_GBitmapFormat format) {
    n_GBitmap* bitmap = n_prv_gbitmap_create(size, format);
    if (bitmap->addr != NULL) {
        memset(bitmap->addr, 0, bitmap->row_size_bytes * size.h);
    }
    return bitmap;
}

n_GBitmap* n_gbitmap_create_blank_with_palette(n_GSize size, n_GBitmapFormat format, n_GColor *palette, bool free_on_destroy) {
    n_GBitmap* bitmap = n_prv_gbitmap_create_with_palette(size, format, palette, free_on_destroy);
    if (bitmap->addr != NULL) {
        memset(bitmap->addr, 0, bitmap->row_size_bytes * size.h);
    }
    return bitmap;
}

n_GBitmap* n_gbitmap_create_palettized_from_1bit(const n_GBitmap *src_bitmap) {
    // the case of src_bitmap not being a 1-bit GBitmap is undefined in the docs
    if (src_bitmap == NULL || src_bitmap->format != n_GBitmapFormat1Bit)
        return NULL;
    n_GBitmap* dst_bitmap = n_prv_gbitmap_create(src_bitmap->bounds.size, n_GBitmapFormat1BitPalette);
    if (dst_bitmap == NULL)
        return NULL;

    dst_bitmap->palette[0] = n_GColorBlack;
    dst_bitmap->palette[1] = n_GColorWhite;
    
    if (dst_bitmap->row_size_bytes == src_bitmap->row_size_bytes) {
        // trivial copy
        memcpy(dst_bitmap->addr, src_bitmap->addr, src_bitmap->row_size_bytes * src_bitmap->bounds.size.h);
    } else if (src_bitmap->bounds.origin.x % 8 == 0) {
        // copy row for row byte-aligned
        int16_t y;
        for (y = 0; y < dst_bitmap->bounds.size.h; y++) {
            memcpy(
                dst_bitmap->addr + y * dst_bitmap->row_size_bytes,
                src_bitmap->addr + src_bitmap->bounds.origin.x + (y + src_bitmap->bounds.origin.y) * src_bitmap->row_size_bytes,
                dst_bitmap->row_size_bytes
            );
        }
    } else {
        // copy row for row non-byte-aligned
        uint8_t off = src_bitmap->bounds.origin.x % 8;
        int16_t x, y;
        uint8_t *dst_pixel = dst_bitmap->addr;
        uint8_t *src_pixel_line = src_bitmap->addr + src_bitmap->bounds.origin.x;
        for (y = 0; y < dst_bitmap->bounds.size.h; y++) {
            uint8_t* src_pixel = src_pixel_line;
            for (x = 0; x < dst_bitmap->bounds.size.w; x++) {
                *dst_pixel = (src_pixel[0] >> off) | (src_pixel[1] << (8 - off));
                dst_pixel++;
                src_pixel++;
            }
            src_pixel_line += src_bitmap->row_size_bytes;
        }
    }

    return dst_bitmap;
}

void n_gbitmap_destroy(n_GBitmap* bitmap) {
    if (bitmap != NULL) {
        if (bitmap->free_data_on_destroy && bitmap->addr) {
            NGFX_PREFERRED_free(bitmap->addr);
        }
        if (bitmap->free_palette_on_destroy && bitmap->palette) {
            NGFX_PREFERRED_free(bitmap->palette);
        }
        NGFX_PREFERRED_free(bitmap);
    }
}