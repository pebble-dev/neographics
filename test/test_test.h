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
