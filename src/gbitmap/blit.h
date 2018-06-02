#pragma once
#include "gbitmap.h"

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                   Blit                                       |
|                                                                              |
|  Because of the complexity of n_graphics_draw_bitmap_in_rect, we break up    |
|  the function in several smaller ones by their overall method of drawing.    |
|  They are then chosen according to the platform, the bitmap pixel format     |
|  and the current compositing operation.                                      |
|  In any case the rects are already clipped and are useful (width,height > 0) |
|                                                                              |
`-----------------------------------------------------------------------------*/

/*! \addtogroup blit
 * Low-level bitmap drawing routines. Not recommended for use.
 * @{
 */

#ifdef PBL_BW

/*!
* Pixels are set one by one with compositing effects applied.
* Used for 1Bit->BW
*/
void n_graphics_blit_comp(struct n_GContext *ctx, const n_GBitmap *bitmap,
    n_GRect bounds, n_GPoint src_offset);

#else

/*!
 * Bitmap pixels can be copied as is and we may do it fast
 * Used for 8Bit->Color with CompOp set to Assign
 */
void n_graphics_blit_mem_copy(struct n_GContext *ctx, const n_GBitmap *bitmap,
    n_GRect bounds, n_GPoint src_offset);

/*!
 * Bitmap pixels are blended one by one into the framebuffer, also: transparency
 * Used for 8Bit->Color with CompOp set to Set
 */
void n_graphics_blit_blend(struct n_GContext *ctx, const n_GBitmap *bitmap,
    n_GRect bounds, n_GPoint src_offset);

/*!
 * Bitmap pixels have to be looked up in the palette and then
 * optionally color blended
 * Used for 1Bit->Color and xBitPalette->Color with any CompOp
 */
void n_graphics_blit_palette(struct n_GContext *ctx, const n_GBitmap *bitmap,
    n_GRect bounds, n_GPoint src_offset);

#endif

/*! @}
*/
