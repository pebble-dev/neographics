#pragma once
#ifdef NGFX_IS_CORE
#include <stdint.h>
#else
#include <pebble.h>
#endif

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                   Point                                      |
|                                                                              |
`-----------------------------------------------------------------------------*/

/*! \addtogroup Types
 *  @{
 *  \addtogroup Point
 *  Defines a point on (or off) the canvas.
 *  @{
 */

/*!
 * Represents a point.
 */
typedef struct n_GPoint {
    int16_t x;
    int16_t y;
} n_GPoint;

/*!
 * Convenience macro to quickly define a point.
 */
#define n_GPoint(x, y) ((n_GPoint) {x, y})
/*!
 * Convenience macro for the origin (top left).
 */
#define n_GPointZero ((n_GPoint) {.x = 0, .y = 0})

/*! @}
 *  @}
 */
