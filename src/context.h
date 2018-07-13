#pragma once
#include <pebble.h>
#include "macros.h"
#include "types.h"
#include "gbitmap/gbitmap.h"

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                  GContext                                    |
|                                                                              |
|   GContexts exist to store information about the graphics environment,       |
|   most importantly a pointer to the framebuffer, information about current   |
|   graphics settings, and information about the currently active graphics     |
|   origin and clipping box.                                                   |
|                                                                              |
`-----------------------------------------------------------------------------*/

/*! \addtogroup context
 *  The graphics context itself.
 *  @{
 */

 //! Values to specify how the source image should be composited onto the destination image.
typedef enum {
    n_GCompOpAssign,         //<! dst = src
    n_GCompOpAssignInverted, //<! dst = ~src
    n_GCompOpOr,             //<! dst = dst | src
    n_GCompOpAnd,            //<! dst = dst & src
    n_GCompOpClear,          //<! dst & ~src
    n_GCompOpSet,            //<! dst | ~src
} n_GCompOp;

/*!
 * Internal representation of the graphics context itself. Created via
 * n_graphics_context_from_buffer() or
 * n_graphics_context_from_graphics_context().
 */
typedef struct n_GContext {
    n_GColor stroke_color;
    n_GColor fill_color;
    n_GColor text_color;
    bool antialias;
    bool stroke_caps;
    uint16_t stroke_width;
    n_GCompOp comp_op;
#ifndef NGFX_IS_CORE
    GContext * underlying_context; // This is necessary where direct framebuffer
                                   // access doens't exist (for example, in the
                                   // PebbleOS development/testing environment).
                                   // In firmwares where neographics _is_ the
                                   // underlying context, this is no longer necessary.
#endif
    GBitmap * bitmap;
    uint8_t * fbuf;
    n_GRect offset;
} n_GContext;

/*!
 * Sets the n_GColor used to draw strokes.
 */
void n_graphics_context_set_stroke_color(n_GContext * ctx, n_GColor color);
/*!
 * Sets the n_GColor used to fill primitives.
 */
void n_graphics_context_set_fill_color(n_GContext * ctx, n_GColor color);
/*!
 * Sets the n_GColor used to draw text.
 */
void n_graphics_context_set_text_color(n_GContext * ctx, n_GColor color);
/*!
 * Sets the compositing mode used to draw bitmaps.
 */
void n_graphics_context_set_compositing_mode(n_GContext * ctx, n_GCompOp comp_op);
/*!
 * Sets whether stroke caps should be drawn.
 */
void n_graphics_context_set_stroke_caps(n_GContext * ctx, bool caps);
/*!
 * Sets the stroke width.
 */
void n_graphics_context_set_stroke_width(n_GContext * ctx, uint16_t width);
/*!
 * Sets whether antialiasing should be used.
 * \note Not implemented yet.
 */
void n_graphics_context_set_antialiased(n_GContext * ctx, bool antialias);

#ifndef NGFX_IS_CORE

/*!
 * In Pebble OS, use this before drawing to the n_GContext for contexts created
 * from a graphics context (via n_graphics_context_from_graphics_context()).
 * Not required on other platforms.
 */
void n_graphics_context_begin(n_GContext * ctx);
/*!
 * In Pebble OS, use this before drawing to the n_GContext for contexts created
 * from a graphics context (via n_graphics_context_from_graphics_context()).
 * Not required on other platforms.
 */
void n_graphics_context_end(n_GContext * ctx);

#else

/*!
 * In native platforms, freeze the framebuffer and return a GBitmap thereof.
 */
GBitmap * n_graphics_capture_frame_buffer(n_GContext * ctx);
/*!
 * In native platforms, freeze the framebuffer and return a GBitmap thereof
 * (using a specified format)
 */
GBitmap * n_graphics_capture_frame_buffer_format(n_GContext * ctx, GBitmapFormat format);
/*!
 * In native platforms, unfreeze the framebuffer.
 */
bool n_graphics_release_frame_buffer(n_GContext * ctx, GBitmap * bitmap);

#endif

/*!
 * Creates a n_GContext based on a framebuffer.
 */
n_GContext * n_graphics_context_from_buffer(uint8_t * buf);

#ifndef NGFX_IS_CORE
/*!
 * Creates a n_GContext based on a GContext (provided by Pebble OS).
 */
n_GContext * n_graphics_context_from_graphics_context(GContext * ctx);
#endif

/*!
 * Destroys a n_GContext.
 */
void n_graphics_context_destroy(n_GContext * ctx);


#ifdef NGFX_IS_CORE
/*!
 * Creates a root n_GContext based on the system framebuffer.
 * This should be done exactly once. ** This context cannoy be freed. **
 */
n_GContext * n_root_graphics_context_from_buffer(uint8_t * buf);
#endif

/*! @}
 */
