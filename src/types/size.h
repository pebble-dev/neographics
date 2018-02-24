#pragma once
#ifdef NGFX_IS_CORE
#include <stdint.h>
#include <stdbool.h>
#else
#include <pebble.h>
#endif

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

/*!
 * Tests whether 2 GSize are equal
 */
bool n_gsize_equal(const n_GSize *size_a, const n_GSize *size_b); // implemented in rect.c

/*! @}
 *  @}
 */
