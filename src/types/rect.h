#pragma once
#ifdef NGFX_IS_CORE
#include <stdint.h>
#include <stdbool.h>
#else
#include <pebble.h>
#endif
#include "point.h"
#include "size.h"

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                    Rect                                      |
|                                                                              |
`-----------------------------------------------------------------------------*/

/*! \addtogroup Types
 *  @{
 *  \addtogroup typerect Rect
 *  @{
 */

typedef struct n_GRect {
    n_GPoint origin;
    n_GSize size;
} n_GRect;

/*!
 * Convenience macro to quickly define a rectangle.
 */
#define n_GRect(x, y, w, h) ((n_GRect) {{x, y}, {w, h}})

/*!
 * Convenience macro to standardize a rectangle.
 * \warning Differs from the Pebble OS implementation, which is in-place and
 * takes a pointer.
 */
#define n_grect_standardize(a) \
    ((a).size.w >= 0 \
        ? (a).size.h >= 0 \
            /* normal */ \
            ? (n_GRect) { {  (a).origin.x,      (a).origin.y }, \
                          {  (a).size.w,        (a).size.h     }} \
            /* switch vertically */ \
            : (n_GRect) { {  (a).origin.x,      (a).origin.y + (a).size.h }, \
                          {  (a).size.w,       -(a).size.h }} \
        : (a).size.h >= 0 \
            /* switch horizontally */ \
            ? (n_GRect) { {  (a).origin.x + (a).size.w, (a).origin.y }, \
                          { -(a).size.w,    (a).size.h     }} \
            /* switch horizontally and vertically */ \
            : (n_GRect) { {  (a).origin.x + (a).size.w, (a).origin.y + (a).size.h }, \
                          { -(a).size.w,   -(a).size.h }})

/*!
 * Returns the point at the center of a GRect (will prefer top and/or left
 * at even sizes)
 */
n_GPoint n_grect_center_point(n_GRect *rect);

/*!
 * Tests whether 2 GRect are equal
 */
bool n_grect_equal(const n_GRect *rect_a, const n_GRect *rect_b);

/*!
 * Tests whether the size of a GRect is (0, 0)
 */
bool n_grect_is_empty(const n_GRect *const rect);

/*!
 * Trim one GRect using the edges of a second GRect
 */
void n_grect_clip(n_GRect *rect_to_clip, const n_GRect *rect_clipper);

/*!
 * Tests whether a GRect contains a point
 */
bool n_grect_contains_point(const n_GRect *rect, const n_GPoint *point);

/*!
 * Reduce the width and height of a GRect and centering it relative to the original
 */
n_GRect n_grect_crop(n_GRect rect, int32_t crop_size_px);

/*! @}
 *  @}
 */
