#pragma once
#include <pebble.h>
#include "macros.h"
#include "types.h"

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
    GBitmap * bitmap;
    uint8_t * fbuf;
#ifndef NGFX_IS_CORE
    GContext * underlying_context; // This is necessary where direct framebuffer
                                   // access doens't exist (for example, in the
                                   // PebbleOS development/testing environment).
                                   // In firmwares where neographics _is_ the
                                   // underlying context, this is no longer necessary.
#endif
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
// void n_graphics_context_set_compositing_mode(n_GContext * ctx, ...);
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

// TODO implement replacement for GBitmap
// n_GBitmap * n_graphics_capture_frame_buffer(n_GContext * ctx);
// n_GBitmap * n_graphics_capture_frame_buffer_format(n_GContext * ctx, n_GBitmapFormat format);
//     // TODO replacement for GBitmapFormat
// bool n_graphics_release_frame_buffer(n_GContext * ctx, n_GBitmap * bitmap);

/*!
 * Creates a n_GContext based on a framebuffer.
 */
n_GContext * n_graphics_context_from_buffer(uint8_t * buf);
/*!
 * Creates a n_GContext based on a GContext (provided by Pebble OS).
 */
n_GContext * n_graphics_context_from_graphics_context(GContext * ctx);
/*!
 * Destroys a n_GContext.
 */
void n_graphics_context_destroy(n_GContext * ctx);


/*! @}
 */
