#pragma once
#include <pebble.h>
#include "types.h"

/*! \addtogroup test
 *  The test framework.
 *  @{
 */

/*!
 * Declares a new test with small footprint.
 */
#define NGFX_TEST(module,name,...)

/*!
 * Declares a new test with big footprint.
 */
#define NGFX_BIG_TEST(module,name,...)

#undef NGFX_TEST // above was just to document
#undef NGFX_BIG_TEST

// Assertions

/*!
 * Asserts some condition with custom message
 */
#define NGFX_ASSERT_MSG(cond,...) do { \
    if (!(cond)) { \
        return (n_TestResult) { \
            .success = false, \
            .message = int_ngfxtest_format_msg(__VA_ARGS__), \
            .file = __FILE__, \
            .line = __LINE__ \
        }; \
    } \
} while(false)

/*!
 * Asserts some condition
 */
#define NGFX_ASSERT(cond) NGFX_ASSERT_MSG((cond), "%s", #cond)

/*!
 * Asserts that some primitive `actual` is equal `expected` with custom message
 */
#define NGFX_ASSERT_EQ_MSG(actual,expected,...) \
    NGFX_ASSERT_MSG((actual) == (expected), __VA_ARGS__)

/*!
 * Asserts that some primitive `actual` is equal `expected`
 */
#define NGFX_ASSERT_EQ(actual,expected) \
    NGFX_ASSERT_EQ_MSG(actual, expected, "%s", "Actual: " #actual " \tExpected: " #expected)

/*!
 * Asserts that some \ref n_GPoint `actual` is equal `expected` with custom message
 */
#define NGFX_ASSERT_POINT_MSG(actual,expected,...) \
    NGFX_ASSERT_MSG( ((actual).x == (expected).x && (actual).y == (expected).y), __VA_ARGS__)

/*!
 * Asserts that some \ref n_GPoint `actual` is equal `expected`
 */
#define NGFX_ASSERT_POINT(actual,expected) \
    NGFX_ASSERT_POINT_MSG(actual, expected, "Actual: (GPoint){%d, %d} \tExpected: (GPoint){%d, %d}", \
        (actual).x, (actual).y, (expected).x, (expected).y)

/*!
 * Asserts that some \ref n_GSize `actual` is equal `expected` with custom message
 */
#define NGFX_ASSERT_SIZE_MSG(actual,expected,...) \
    NGFX_ASSERT_MSG( ((actual).w == (expected).w && (actual).h == (expected).h), __VA_ARGS__)

/*!
 * Asserts that some \ref n_GSize `actual` is equal `expected`
 */
#define NGFX_ASSERT_SIZE(actual,expected) \
    NGFX_ASSERT_SIZE_MSG(actual, expected, "Actual: (GSize){%d, %d} \tExpected: (GSize){%d, %d}", \
        (actual).w, (actual).h, (expected).w, (expected).h)

/*!
 * Asserts that some \ref n_GRect `actual` is equal `expected` with custom message
 */
#define NGFX_ASSERT_RECT_MSG(actual,expected,...) \
    NGFX_ASSERT_MSG( ((actual).origin.x == (expected).origin.x && (actual).origin.y == (expected).origin.y && \
        (actual).size.w == (expected).size.w && (actual).size.h == (expected).size.h), __VA_ARGS__)

/*!
 * Asserts that some \ref n_GRect `actual` is equal `expected`
 */
#define NGFX_ASSERT_RECT(actual,expected) \
    NGFX_ASSERT_RECT_MSG(actual, expected, "Actual: (GRect){%d, %d, %d, %d} \tExpected: (GRect){%d, %d, %d, %d}", \
        (actual).origin.x, (actual).origin.y, (actual).size.w, (actual).size.h, \
        (expected).origin.x, (expected).origin.y, (expected).size.w, (expected).size.h)

/*!
 * Asserts that some \ref n_GColor `actual` is equal `expected` with custom message
 */
#define NGFX_ASSERT_COLOR_MSG(actual,expected,...) \
    NGFX_ASSERT_MSG( ((actual).argb == (expected).argb), __VA_ARGS__)

/*!
 * Asserts that some \ref n_GColor `actual` is equal `expected`
 */
#define NGFX_ASSERT_COLOR(actual,expected,...) \
    NGFX_ASSERT_COLOR_MSG(actual, expected, "Actual: (GColor){%d, %d, %d, %d} \tExpected: (GColor){%d, %d, %d, %d}", \
        (actual).r, (actual).g, (actual).b, (actual).a, (expected).r, (expected).g, (expected).b, (expected).a)

/*!
 * Asserts that the current framebuffer pixel at `point` is of color `expected_color` with custom message
 */
#define NGFX_ASSERT_PIXEL_MSG(point,expected_color,...) \
    NGFX_ASSERT_MSG(int_ngfxtest_pixel_eq(point, expected_color), __VA_ARGS__)

/*!
 * Asserts that the current framebuffer pixel at `point` is of color `expected_color`
 */
#define NGFX_ASSERT_PIXEL(point,expected_color) \
    NGFX_ASSERT_PIXEL_MSG(point, expected_color, "%s", int_ngfxtest_msg_pixel(point, expected_color))

/*!
 * Asserts that the current framebuffer area `rect` is equal to the resource image `expected_resource` with custom message
 * \note `expected_resource` is the id of a mapped resource
 */
#define NGFX_ASSERT_SUBSCREEN_MSG(rect,expected_resource,...) \
    NGFX_ASSERT_MSG(int_ngfxtest_subscreen_eq(rect, expected_resource), __VA_ARGS__)

 /*!
 * Asserts that the current framebuffer area `rect` is equal to the resource image `expected_resource`
 * \note `expected_resource` is the id of a mapped resource
 */
#define NGFX_ASSERT_SUBSCREEN(rect,expected_resource) \
    NGFX_ASSERT_SUBSCREEN_MSG(rect, expected_resource, "%s", int_ngfxtest_msg_subscreen(rect, expected_resource_name))

 /*!
 * Asserts that the full current framebuffer is equal to the resource image `expected_resource` with custom message
 * \note `expected_resource` is the id of a mapped resource
 */
#define NGFX_ASSERT_SCREEN_MSG(expected_resource,...) \
    NGFX_ASSERT_SUBSCREEN_MSG((n_GRect){ 0, 0, __SCREEN_WIDTH, __SCREEN_HEIGHT }, expected_resource, __VA_ARGS__)

 /*!
 * Asserts that the full current framebuffer is equal to the resource image `expected_resource`
 * \note `expected_resource` is the id of a mapped resource
 */
#define NGFX_ASSERT_SCREEN(expected_resource) \
    NGFX_ASSERT_SCREEN_MSG(expected_resource, "%s", int_ngfxtest_msg_subscreen(rect, expected_resource))

// Resources

/*!
 * Maps a resource to a specific resource id (can overwrite old mappings)
 */
#define ngfxtest_map_resource(resource_name,resource_id)  \
    NGFX_ASSERT_MSG(int_ngfxtest_map_resource((resource_name), (resource_id)), \
        "Could not load resource \"" resource_name "\"")

/*! @}
 */

// Internal (do not use in tests)
#ifndef DOXYGEN_SHOULD_SKIP_THIS
bool int_ngfxtest_pixel_eq(n_GPoint point, n_GColor expected_color);
bool int_ngfxtest_subscreen_eq(n_GRect rect, uint32_t expected_resource_id);

const char* int_ngfxtest_format_msg(const char* format, ...);
const char* int_ngfxtest_msg_pixel(n_GPoint point, n_GColor expected_color);
const char* int_ngfxtest_msg_subscreen(n_GRect rect, uint32_t expected_resource_id);

bool int_ngfxtest_map_resource(const char* resource_name, uint32_t resource_id);

typedef struct {
    bool success;
    const char* message;
    const char* file;
    unsigned int line;
} n_TestResult;

typedef n_TestResult (*n_TestFunction)(void);

typedef struct {
    const char* module;
    const char* name;
    n_TestFunction func;
} n_Test;
#endif
