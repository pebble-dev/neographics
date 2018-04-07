#include "test.h"
#include "gbitmap/gbitmap.h"

NGFX_TEST(GBitmap, CreateBlank,
    const uint8_t expected_data[16] = { 0 };
    n_GBitmap* bitmap;

    bitmap = n_gbitmap_create_blank((n_GSize) { 3, 4 }, n_GBitmapFormat8Bit);
    NGFX_ASSERT(bitmap != NULL);
    NGFX_ASSERT_EQ(n_gbitmap_get_format(bitmap), n_GBitmapFormat8Bit);
    NGFX_ASSERT_EQ(n_gbitmap_get_bytes_per_row(bitmap), 3);
    NGFX_ASSERT_RECT(n_gbitmap_get_bounds(bitmap), n_GRect(0, 0, 3, 4));

    uint8_t* actual_data = n_gbitmap_get_data(bitmap);
    NGFX_ASSERT(actual_data != NULL);
    NGFX_ASSERT_MSG(memcmp(actual_data, expected_data, 3 * 4) == 0, "Bitmap was not blanked");

    n_gbitmap_destroy(bitmap);
)

NGFX_TEST(GBitmap, CreateBlankWithPalette,
    const uint8_t expected_data[16] = { 0 };
    n_GColor palette[] = { n_GColorWhite, n_GColorBlack, n_GColorBlue, n_GColorRed };
    n_GBitmap* bitmap;

    bitmap = n_gbitmap_create_blank_with_palette((n_GSize) { 7, 4 }, n_GBitmapFormat2BitPalette, palette, false);
    NGFX_ASSERT(bitmap != NULL);
    NGFX_ASSERT_EQ(n_gbitmap_get_format(bitmap), n_GBitmapFormat2BitPalette);
    NGFX_ASSERT_EQ(n_gbitmap_get_bytes_per_row(bitmap), 2);
    NGFX_ASSERT_RECT(n_gbitmap_get_bounds(bitmap), n_GRect(0, 0, 7, 4));

    uint8_t* actual_data = n_gbitmap_get_data(bitmap);
    NGFX_ASSERT(actual_data != NULL);
    NGFX_ASSERT_MSG(memcmp(actual_data, expected_data, 2 * 4) == 0, "Bitmap was not blanked");

    n_gbitmap_destroy(bitmap);
)

NGFX_TEST(GBitmap, CreateAsSubBitmap,
    ngfxtest_map_resource("test_checker.png", 1);
    ngfxtest_load_image(base_bitmap, 1);
    const uint8_t* base_data = n_gbitmap_get_data(base_bitmap);
    uint8_t base_data_backup[4];
    memcpy(base_data_backup, base_data, 4);
    n_GBitmap* sub_bitmap;

    // Full 
    sub_bitmap = n_gbitmap_create_as_sub_bitmap(base_bitmap, n_GRect(0, 0, 2, 2));
    NGFX_ASSERT(sub_bitmap != NULL);
    NGFX_ASSERT_EQ(n_gbitmap_get_format(sub_bitmap), n_GBitmapFormat8Bit);
    NGFX_ASSERT_RECT(n_gbitmap_get_bounds(sub_bitmap), n_GRect(0, 0, 2, 2));
    NGFX_ASSERT_EQ(n_gbitmap_get_data(sub_bitmap), base_data);
    n_gbitmap_destroy(sub_bitmap);
    NGFX_ASSERT_MSG(memcmp(base_data, base_data_backup, 4) == 0, "Base bitmap data was modified");

    // Sub
    sub_bitmap = n_gbitmap_create_as_sub_bitmap(base_bitmap, n_GRect(1, 1, 1, 1));
    NGFX_ASSERT(sub_bitmap != NULL);
    NGFX_ASSERT_RECT(n_gbitmap_get_bounds(sub_bitmap), n_GRect(1, 1, 1, 1));
    n_gbitmap_destroy(sub_bitmap);

    // Oversize
    sub_bitmap = n_gbitmap_create_as_sub_bitmap(base_bitmap, n_GRect(-1, -1, 5, 5));
    NGFX_ASSERT(sub_bitmap != NULL);
    NGFX_ASSERT_RECT(n_gbitmap_get_bounds(sub_bitmap), n_GRect(0, 0, 2, 2));
    n_gbitmap_destroy(sub_bitmap);
)

NGFX_TEST(GBitmap, SetData,
    uint8_t data[] = { 0x01, 0x23 };
    n_GColor palette[] = { n_GColorBlue, n_GColorBlack, n_GColorWhite, n_GColorRed };
    n_GBitmap* bitmap = n_gbitmap_create_blank((n_GSize) { 1, 1 }, n_GBitmapFormat8Bit);

    n_gbitmap_set_data(bitmap, data, n_GBitmapFormat4BitPalette, 2, false);
    NGFX_ASSERT_EQ(n_gbitmap_get_bytes_per_row(bitmap), 2);
    NGFX_ASSERT_EQ(n_gbitmap_get_format(bitmap), n_GBitmapFormat4BitPalette);
    NGFX_ASSERT_EQ(n_gbitmap_get_data(bitmap), data); // the addresses should be equal

    n_gbitmap_set_palette(bitmap, palette, false);
    NGFX_ASSERT_EQ(n_gbitmap_get_palette(bitmap), palette);

    n_GRect bounds = n_GRect(0, 0, 2, 2);
    n_gbitmap_set_bounds(bitmap, bounds);
    NGFX_ASSERT_RECT(n_gbitmap_get_bounds(bitmap), bounds);

    n_gbitmap_destroy(bitmap);
)

NGFX_TEST(GBitmap, CreatePalettizedFrom1Bit,
    uint8_t base_data[] = { // a checkerboard style 16x4 1-bit image
        0b01010011, 0b00001111,
        0b10101100, 0b11110000,
        0b00110011, 0b00001111,
        0b00110011, 0b00001111
    };
    n_GBitmap* base_bitmap = n_gbitmap_create_blank((n_GSize) { 1, 1 }, n_GBitmapFormat8Bit);
    n_gbitmap_set_data(base_bitmap, base_data, n_GBitmapFormat1Bit, 2, false);
    n_gbitmap_set_bounds(base_bitmap, n_GRect(0, 0, 16, 4));

    // Full bitmap (byte-aligned copy)
    {
        const n_GColor expected_palette[2] = { n_GColorBlack, n_GColorWhite };
        n_GBitmap* pal_bitmap = n_gbitmap_create_palettized_from_1bit(base_bitmap);
        NGFX_ASSERT(pal_bitmap != NULL);

        NGFX_ASSERT_EQ(n_gbitmap_get_format(pal_bitmap), n_GBitmapFormat1BitPalette);
        NGFX_ASSERT_RECT(n_gbitmap_get_bounds(pal_bitmap), n_GRect(0, 0, 16, 4));
        NGFX_ASSERT_EQ(n_gbitmap_get_bytes_per_row(pal_bitmap), 2);

        n_GColor* palette = n_gbitmap_get_palette(pal_bitmap);
        NGFX_ASSERT(palette != NULL);
        NGFX_ASSERT(memcmp(palette, expected_palette, 2) == 0);

        uint8_t* actual_data = n_gbitmap_get_data(pal_bitmap);
        NGFX_ASSERT(actual_data != NULL);
        NGFX_ASSERT(memcmp(actual_data, base_data, 2 * 4) == 0);
        n_gbitmap_destroy(pal_bitmap);
    }

    // Sub bitmap (byte-aligned copy)
    {
        const uint8_t expected_data[] = {
            0b11110000,
            0b00001111
        };
        n_GBitmap* sub_bitmap = n_gbitmap_create_as_sub_bitmap(base_bitmap, n_GRect(8, 1, 4, 2));
        NGFX_ASSERT(sub_bitmap != NULL);
        n_GBitmap* pal_bitmap = n_gbitmap_create_palettized_from_1bit(sub_bitmap);
        NGFX_ASSERT(pal_bitmap != NULL);
        n_gbitmap_destroy(sub_bitmap);

        NGFX_ASSERT_RECT(n_gbitmap_get_bounds(pal_bitmap), n_GRect(0, 0, 4, 2));
        NGFX_ASSERT_EQ(n_gbitmap_get_bytes_per_row(pal_bitmap), 1);
        uint8_t* actual_data = n_gbitmap_get_data(pal_bitmap);
        NGFX_ASSERT(actual_data != NULL);
        NGFX_ASSERT_MEM(actual_data, expected_data, 1 * 2);
        n_gbitmap_destroy(pal_bitmap);
    }

    // Sub bitmap (non-byte-aligned copy)
    {
        const uint8_t expected_data[] = {
            0b00010101, 0b00001110, // if confused, translate into an image editor
            0b11100110, 0b00000001  // remember lowest bit -> first pixel
        };
        n_GBitmap* sub_bitmap = n_gbitmap_create_as_sub_bitmap(base_bitmap, n_GRect(3, 1, 12, 2));
        NGFX_ASSERT(sub_bitmap != NULL);
        n_GBitmap* pal_bitmap = n_gbitmap_create_palettized_from_1bit(sub_bitmap);
        NGFX_ASSERT(pal_bitmap != NULL);
        n_gbitmap_destroy(sub_bitmap);

        NGFX_ASSERT_RECT(n_gbitmap_get_bounds(pal_bitmap), n_GRect(0, 0, 12, 2));
        NGFX_ASSERT_EQ(n_gbitmap_get_bytes_per_row(pal_bitmap), 2);
        uint8_t* actual_data = n_gbitmap_get_data(pal_bitmap);
        NGFX_ASSERT(actual_data != NULL);
        NGFX_ASSERT_MEM(actual_data, expected_data, 2 * 2);
        n_gbitmap_destroy(pal_bitmap);
    }

    // Sub bitmap (non-byte-aligned copy)
    {
        const uint8_t expected_data[] = {
            0b00010101,
            0b01100110,
        };
        n_GBitmap* sub_bitmap = n_gbitmap_create_as_sub_bitmap(base_bitmap, n_GRect(3, 1, 7, 2));
        NGFX_ASSERT(sub_bitmap != NULL);
        n_GBitmap* pal_bitmap = n_gbitmap_create_palettized_from_1bit(sub_bitmap);
        NGFX_ASSERT(pal_bitmap != NULL);
        n_gbitmap_destroy(sub_bitmap);

        NGFX_ASSERT_RECT(n_gbitmap_get_bounds(pal_bitmap), n_GRect(0, 0, 7, 2));
        NGFX_ASSERT_EQ(n_gbitmap_get_bytes_per_row(pal_bitmap), 1);
        uint8_t* actual_data = n_gbitmap_get_data(pal_bitmap);
        NGFX_ASSERT(actual_data != NULL);
        NGFX_ASSERT_MEM(actual_data, expected_data, 1 * 2);
        n_gbitmap_destroy(pal_bitmap);
    }
)
