#pragma once

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                 CornerMask                                   |
|                                                                              |
`-----------------------------------------------------------------------------*/

/*! \addtogroup Types
 *  @{
 *  \addtogroup CornerMask
 *  @{
 */

/*!
 * Describes what corners should be rounded when drawing a shape.
 */
typedef enum n_GCornerMask {
    n_GCornersNone       = 0, // support all grammatical preferences
    n_GCornerNone        = 0,
    n_GCornerTopLeft     = 0b0001,
    n_GCornerTopRight    = 0b0010,
    n_GCornerBottomLeft  = 0b0100,
    n_GCornerBottomRight = 0b1000,
    n_GCornersTop        = 0b0011,
    n_GCornersBottom     = 0b1100,
    n_GCornersLeft       = 0b0101,
    n_GCornersRight      = 0b1010,
    n_GCornersAll        = 0b1111,
} n_GCornerMask;

/*! @}
 *  @}
 */
