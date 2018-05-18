#pragma once
#include <pebble.h>
#include "types.h"

//! The format of a GBitmap can either be 1-bit or 8-bit.
typedef enum n_GBitmapFormat {
    n_GBitmapFormat1Bit = 0,     //<! 1-bit black and white. 0 = black, 1 = white.
    n_GBitmapFormat8Bit,         //<! 6-bit color + 2 bit alpha channel. See \ref GColor8 for pixel format.
    n_GBitmapFormat1BitPalette,  //<! up to 2 8-bit colors in the palette, 1-bit indices as pixel data
    n_GBitmapFormat2BitPalette,  //<! up to 4 8-bit colors in the palette, 2-bit indices as pixel data
    n_GBitmapFormat4BitPalette,  //<! up to 16 8-bit colors in the palette, 4-bit indices as pixel data
    n_GBitmapFormat8BitCircular, //<! undocumented GBitmap format, currently behaves like \ref GBitmapFormat8Bit
} n_GBitmapFormat;

//! The n_GBitmap structure
typedef struct n_GBitmap
{
    uint8_t *addr;
    n_GSize raw_bitmap_size;
    n_GColor *palette;
    uint8_t palette_size;
    uint16_t row_size_bytes;
    bool free_palette_on_destroy; // TODo move me to a bit status register above for size
    bool free_data_on_destroy; // TODo move me to a bit status register above for size
    n_GRect bounds;
    n_GBitmapFormat format;
} n_GBitmap;

//! Get the number of bytes per row in the bitmap data for the given \ref n_GBitmap.
//! @param bitmap A pointer to the n_GBitmap
//! @return The number of bytes per row
uint16_t n_gbitmap_get_bytes_per_row(const n_GBitmap *bitmap);

//! Get the \ref n_GBitmapFormat for the \ref n_GBitmap.
//! @param bitmap A pointer to a n_GBitmap
//! @return The format of the given \ref n_GBitmap.
n_GBitmapFormat n_gbitmap_get_format(const n_GBitmap *bitmap);

//! Get a pointer to the raw image data section of the given \ref n_GBitmap as
//! specified by the format of the bitmap.
//! @param bitmap A pointer to a n_GBitmap
//! @return pointer to the raw image data
uint8_t *n_gbitmap_get_data(const n_GBitmap *bitmap);

//! Set the raw bitmap data for the given \ref n_GBitmap.
//! @param bitmap A pointer to the n_GBitmap
//! @param data A pointer to the raw bitmap data
//! @param format the format of the  data
//! @param row_size_bytes How many bytes a single row takes
//! @param free_on_destroy Set whether the data should be freed when the n_GBitmap is destroyed.
void n_gbitmap_set_data(n_GBitmap *bitmap, uint8_t *data, n_GBitmapFormat format, uint16_t row_size_bytes, bool free_on_destroy);

//! Gets the bounds of the content for the \ref n_GBitmap
//! @param bitmap A pointer to the n_GBitmap
//! @return The bounding box for the n_GBitmap
n_GRect n_gbitmap_get_bounds(const n_GBitmap *bitmap);

//! Set the bounds of the given \ref n_GBitmap.
//! @param bitmap A pointer to the n_GBitmap
//! @param bounds The bounding box to set.
void n_gbitmap_set_bounds(n_GBitmap *bitmap, n_GRect bounds);

//! Get the palette for the given \ref GBitmap.
//! @param bitmap A pointer to the GBitmap
//! @return Pointer to a \ref n_GColor array containing the palette colors.
n_GColor *n_gbitmap_get_palette(const n_GBitmap *bitmap);

//! Set the palette for the given \ref n_GBitmap.
//! @param bitmap A pointer to the n_GBitmap
//! @param palette The palette to be used
//! @param free_on_destroy Set whether the palette data should be freed when the GBitmap is
//! destroyed or when another palette is set.
void n_gbitmap_set_palette(n_GBitmap *bitmap, n_GColor *palette, bool free_on_destroy);

//! Create a new \ref n_GBitmap on the heap as a sub-bitmap of a 'base' \ref
//! n_GBitmap, using a n_GRect to indicate what portion of the base to use. The
//! sub-bitmap will just reference the image data and palette of the base bitmap,
//! which has to keep valid as long as the sub-bitmap is used.
//! @param base_bitmap The bitmap that the sub-bitmap of which the image data
//! will be used by the sub-bitmap
//! @param sub_rect The rectangle within the image data of the base bitmap. The
//! bounds of the base bitmap will be used to clip `sub_rect`.
//! @return A pointer to the \ref n_GBitmap or `NULL`.
n_GBitmap *n_gbitmap_create_as_sub_bitmap(const n_GBitmap *base_bitmap, n_GRect sub_rect);

//! Creates a new blank n_GBitmap on the heap initialized to zeroes.
//! @param size The Pebble image dimensions as a \ref n_GSize.
//! @param format The \ref n_GBitmapFormat the created image should be in.
//! @return A pointer to the \ref n_GBitmap or `NULL`.
n_GBitmap *n_gbitmap_create_blank(n_GSize size, n_GBitmapFormat format);

//! Creates a new blank n_GBitmap on the heap, initialized to zeroes, and assigns it the given
//! palette, which has to be kept valid.
//! @param size The Pebble image dimensions as a \ref GSize.
//! @param format the \ref n_GBitmapFormat the created image and palette should be in.
//! @param palette a pointer to a palette that is to be used for this n_GBitmap.
//! @param free_on_destroy Set whether the palette data should be freed along with the bitmap data
//! when the n_GBitmap is destroyed.
//! @return A Pointer to the \ref n_GBitmap or `NULL`.
n_GBitmap *n_gbitmap_create_blank_with_palette(n_GSize size, n_GBitmapFormat format, n_GColor *palette, bool free_on_destroy);

//! Given a 1-bit GBitmap, create a new bitmap of format n_GBitmapFormat1BitPalette.
//! The new data buffer is allocated on the heap, and a 2-color palette is allocated as well.
//! @param src_bitmap A n_GBitmap of format n_GBitmapFormat1Bit which is to be copied into a newly
//! created n_GBitmap of format n_GBitmapFormat1BitPalettized.
//! @returns The newly created 1-bit palettized n_GBitmap, or NULL if there is not sufficient space.
n_GBitmap *n_gbitmap_create_palettized_from_1bit(const n_GBitmap *src_bitmap);

//! Destroy a \ref n_GBitmap.
void n_gbitmap_destroy(n_GBitmap *bitmap);
