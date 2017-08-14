#pragma once
#include <pebble.h>

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                    Size                                      |
|                                                                              |
`-----------------------------------------------------------------------------*/

/*! \addtogroup Types
 *  @{
 *  \addtogroup Size
 *  Defines a size, which contains information about something's width and
 *  height.
 *  @{
 */

typedef struct n_GSize {
    int16_t w;
    int16_t h;
} n_GSize;

/*! @}
 *  @}
 */
