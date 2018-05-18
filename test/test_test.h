#include "test.h"
#include "common.h"

NGFX_TEST(Test, EmptyTest, {
})

NGFX_TEST(Test, SimpleAsserts, {
    NGFX_ASSERT(true);
    NGFX_ASSERT(false != true);
    NGFX_ASSERT(3 < 4);

    n_GPoint point = (n_GPoint) { .x = 3, .y = 4 };
    NGFX_ASSERT_POINT(point, point);

    n_GRect rect = (n_GRect) { .origin = { .x = 1, .y = 2 }, .size = { .w = 3, .h = 4 } };
    NGFX_ASSERT_RECT(rect, rect);

    uint8_t data[4] = { 0x12, 0x34, 0x56, 0x78 };
    NGFX_ASSERT_MEM(data, data, 4);
    NGFX_ASSERT_MEM(data, data, 0);
})

NGFX_TEST(Test, FailSimpleAssert, {
    NGFX_ASSERT(false);
})

NGFX_TEST(Test, FailPointAssert, {
    n_GPoint a = (n_GPoint) { .x = 1, .y = 2 };
    n_GPoint b = (n_GPoint) { .x = 1, .y = 3 };
    NGFX_ASSERT_POINT(a, b);
})

NGFX_TEST(Test, FailRectAssert, {
    n_GRect a = (n_GRect) { .origin = { .x = 1, .y = 2 },.size = { .w = 3, .h = 4 } };
    n_GRect b = (n_GRect) { .origin = { .x = 1, .y = 5 }, .size = { .w = 9, .h = 4 } };
    NGFX_ASSERT_RECT(a, b);
})

NGFX_TEST(Test, FailMemAssert, {
    uint8_t data1[4] = { 0x12, 0x34, 0x56, 0x78 };
    uint8_t data2[4] = { 0x12, 0x34, 0x56, 0x7F };
    NGFX_ASSERT_MEM(data1, data2, 4);
})

NGFX_TEST(Test, SimpleResource, {
    ngfxtest_map_resource("test_helloworld.txt", 1);
    const char *const expected = "Hello World!";

    uint8_t buffer[64];
    ResHandle handle = resource_get_handle(1);
    NGFX_ASSERT(handle != NULL);
    size_t size = resource_size(handle);
    NGFX_ASSERT_EQ(size, strlen(expected));
    size_t read = resource_load(handle, buffer, 64);
    NGFX_ASSERT_EQ(read, strlen(expected));
    NGFX_ASSERT(memcmp(buffer, expected, strlen(expected)) == 0);
})

NGFX_TEST(Test, FailNonExistantResource, {
    ngfxtest_map_resource("nonexist", 1);
})

NGFX_TEST(Test, ScreenAssert, {
    ngfxtest_map_resource("test_checker_full.png", 1);
    
    n_graphics_set_pixel(context, n_GPoint(80, 80), n_GColorWhite);
    n_graphics_set_pixel(context, n_GPoint(81, 80), n_GColorBlack);
#ifdef PBL_BW
    n_graphics_set_pixel(context, n_GPoint(80, 81), n_GColorBlack);
    n_graphics_set_pixel(context, n_GPoint(81, 81), n_GColorWhite);
#else
    n_graphics_set_pixel(context, n_GPoint(80, 81), n_GColorRed);
    n_graphics_set_pixel(context, n_GPoint(81, 81), n_GColorGreen);
#endif

    NGFX_ASSERT_SCREEN(1);
})

NGFX_TEST(Test, SubScreenAssert, {
    ngfxtest_map_resource("test_checker.png", 1);
    
    n_graphics_set_pixel(context, n_GPoint(80, 80), n_GColorWhite);
    n_graphics_set_pixel(context, n_GPoint(81, 80), n_GColorBlack);
#ifdef PBL_BW
    n_graphics_set_pixel(context, n_GPoint(80, 81), n_GColorBlack);
    n_graphics_set_pixel(context, n_GPoint(81, 81), n_GColorWhite);
#else
    n_graphics_set_pixel(context, n_GPoint(80, 81), n_GColorRed);
    n_graphics_set_pixel(context, n_GPoint(81, 81), n_GColorGreen);
#endif

    NGFX_ASSERT_SUBSCREEN(n_GRect(80, 80, 2, 2), 1);
})

NGFX_TEST(Test, FailScreenOutOfBounds, {
    ngfxtest_map_resource("test_checker.png", 1);
    
    n_graphics_set_pixel(context, n_GPoint(80, 80), n_GColorWhite);
    n_graphics_set_pixel(context, n_GPoint(81, 80), n_GColorBlack);
#ifdef PBL_BW
    n_graphics_set_pixel(context, n_GPoint(80, 81), n_GColorBlack);
    n_graphics_set_pixel(context, n_GPoint(81, 81), n_GColorWhite);
#else
    n_graphics_set_pixel(context, n_GPoint(80, 81), n_GColorRed);
    n_graphics_set_pixel(context, n_GPoint(81, 81), n_GColorGreen);
#endif

    NGFX_ASSERT_SUBSCREEN(n_GRect(-1, -1, 900, 900), 1);
})

NGFX_TEST(Test, FailScreenUnmapped, {
    NGFX_ASSERT_SUBSCREEN(n_GRect(0, 0, 2, 2), 1);
})

NGFX_TEST(Test, FailScreenImageSize, {
    ngfxtest_map_resource("test_checker.png", 1);
    
    n_graphics_set_pixel(context, n_GPoint(80, 80), n_GColorWhite);
    n_graphics_set_pixel(context, n_GPoint(81, 80), n_GColorBlack);
#ifdef PBL_BW
    n_graphics_set_pixel(context, n_GPoint(80, 81), n_GColorBlack);
    n_graphics_set_pixel(context, n_GPoint(81, 81), n_GColorWhite);
#else
    n_graphics_set_pixel(context, n_GPoint(80, 81), n_GColorRed);
    n_graphics_set_pixel(context, n_GPoint(81, 81), n_GColorGreen);
#endif

    NGFX_ASSERT_SUBSCREEN(n_GRect(80, 80, 5, 5), 1);
})

NGFX_TEST(Test, FailScreenWrongColor, {
    ngfxtest_map_resource("test_checker.png", 1);

    NGFX_ASSERT_SUBSCREEN(n_GRect(0, 0, 2, 2), 1);
})

NGFX_TEST(Test, LoadImage,
    ngfxtest_map_resource("test_checker.png", 1);
    {
        ngfxtest_load_image(checker, 1);

        NGFX_ASSERT_SIZE(checker->raw_bitmap_size, ((n_GSize) { 2, 2 }));
        NGFX_ASSERT_RECT(checker->bounds, n_GRect(0, 0, 2, 2));
        NGFX_ASSERT_EQ(checker->format, n_GBitmapFormat8Bit);
        NGFX_ASSERT_EQ(checker->addr[0], n_GColorWhite.argb);
        NGFX_ASSERT_EQ(checker->addr[1], n_GColorBlack.argb);
        NGFX_ASSERT_EQ(checker->addr[2], n_GColorRed.argb);
        NGFX_ASSERT_EQ(checker->addr[3], n_GColorGreen.argb);
    }
    {
        ngfxtest_load_image_ex(checker, 1, n_GBitmapFormat4BitPalette);
        n_GColor colors[4] = {
            checker->palette[checker->addr[0] & 15],
            checker->palette[checker->addr[0] >> 4],
            checker->palette[checker->addr[1] & 15],
            checker->palette[checker->addr[1] >> 4]
        };

        NGFX_ASSERT_EQ(checker->format, n_GBitmapFormat4BitPalette);
        NGFX_ASSERT_COLOR(colors[0], n_GColorWhite);
        NGFX_ASSERT_COLOR(colors[1], n_GColorBlack);
        NGFX_ASSERT_COLOR(colors[2], n_GColorRed);
        NGFX_ASSERT_COLOR(colors[3], n_GColorGreen);
    }
    {
        ngfxtest_load_image_ex(checker, 1, n_GBitmapFormat2BitPalette);
        n_GColor colors[4] = {
            checker->palette[(checker->addr[0] >> 0) & 3],
            checker->palette[(checker->addr[0] >> 2) & 3],
            checker->palette[(checker->addr[1] >> 0) & 3],
            checker->palette[(checker->addr[1] >> 2) & 3]
        };

        NGFX_ASSERT_EQ(checker->format, n_GBitmapFormat2BitPalette);
        NGFX_ASSERT_COLOR(colors[0], n_GColorWhite);
        NGFX_ASSERT_COLOR(colors[1], n_GColorBlack);
        NGFX_ASSERT_COLOR(colors[2], n_GColorRed);
        NGFX_ASSERT_COLOR(colors[3], n_GColorGreen);
    }
    {
        ngfxtest_load_image_ex(checker, 1, n_GBitmapFormat1BitPalette);
        n_GColor colors[4] = {
            checker->palette[(checker->addr[0] >> 0) & 1],
            checker->palette[(checker->addr[0] >> 1) & 1],
            checker->palette[(checker->addr[1] >> 0) & 1],
            checker->palette[(checker->addr[1] >> 1) & 1]
        };

        NGFX_ASSERT_EQ(checker->format, n_GBitmapFormat1BitPalette);
        NGFX_ASSERT_COLOR(colors[0], n_GColorWhite);
        NGFX_ASSERT_COLOR(colors[1], n_GColorBlack);
        NGFX_ASSERT_COLOR(colors[2], n_GColorBlack);
        NGFX_ASSERT_COLOR(colors[3], n_GColorBlack);
    }
    {
        ngfxtest_load_image_ex(checker, 1, n_GBitmapFormat1Bit);

        NGFX_ASSERT_EQ(checker->format, n_GBitmapFormat1Bit);
        NGFX_ASSERT_EQ(checker->addr[0] & 3, 0b01);
        NGFX_ASSERT_EQ(checker->addr[1] & 3, 0b00);
    }
)
