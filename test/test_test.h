#include "test.h"

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

NGFX_TEST(Test, SimpleResource, {
    ngfxtest_map_resource("test_helloworld.txt", 1);
    const char* const expected = "Hello World!";

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
