#include "test.h"

NGFX_TEST(Types, GRectStandardize, {
    n_GRect actual;

    n_GRect simple = n_GRect(1, 2, 3, 4);
    actual = n_grect_standardize(simple);
    NGFX_ASSERT_RECT(actual, n_GRect(1, 2, 3, 4));

    n_GRect negative_width = n_GRect(1, 2, -3, 4);
    actual = n_grect_standardize(negative_width);
    NGFX_ASSERT_RECT(actual, n_GRect(-2, 2, 3, 4));

    n_GRect negative_height = n_GRect(1, 2, 3, -4);
    actual = n_grect_standardize(negative_height);
    NGFX_ASSERT_RECT(actual, n_GRect(1, -2, 3, 4));

    n_GRect negative_size = n_GRect(20, 20, -10, -5);
    actual = n_grect_standardize(negative_size);
    NGFX_ASSERT_RECT(actual, n_GRect(10, 15, 10, 5));

    n_GRect zero_size = n_GRect(7, 8, 0, 0);
    actual = n_grect_standardize(zero_size);
    NGFX_ASSERT_RECT(actual, n_GRect(7, 8, 0, 0));
})

NGFX_TEST(Types, GRectCenterPoint, {
    n_GPoint actual;

    n_GRect even = n_GRect(1, 2, 6, 4);
    actual = n_grect_center_point(&even);
    NGFX_ASSERT_POINT(actual, n_GPoint(4, 4));
     
    n_GRect odd = n_GRect(5, 6, 7, 9);
    actual = n_grect_center_point(&odd);
    NGFX_ASSERT_POINT(actual, n_GPoint(8, 10));

    n_GRect negative = n_GRect(2, 3, -4, -5);
    actual = n_grect_center_point(&negative);
    NGFX_ASSERT_POINT(actual, n_GPoint(0, 1));

    n_GRect zero = n_GRect(10, 12, 0, 0);
    actual = n_grect_center_point(&zero);
    NGFX_ASSERT_POINT(actual, n_GPoint(10, 12));
})
