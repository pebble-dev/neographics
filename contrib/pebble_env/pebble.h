#ifndef _PEBBLE_H
#define _PEBBLE_H
/* enough emulation to run neographics locally, except math
 * all functions are expected to be defined in the importing
 * project (e.g. test_neographics_rect)
 */

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// Math

#define TRIG_MAX_RATIO 0xffff
#define TRIG_MAX_ANGLE 0x10000

int32_t sin_lookup(int32_t angle);
int32_t cos_lookup(int32_t angle);

// Resource
typedef void *ResHandle;

size_t resource_size(ResHandle handle);
ResHandle resource_get_handle(uint32_t id);
size_t resource_load(ResHandle h, uint8_t * buffer, size_t max_length);

// GBitmap

typedef enum GAlign
{
	GAlignCenter,
	GAlignTopLeft,
	GAlignTopRight,
	GAlignTop,
	GAlignLeft,
	GAlignBottom,
	GAlignRight,
	GAlignBottomRight,
	GAlignBottomLeft
} GAlign;


typedef enum GCompOp {
	GCompOpAssign = 0,
	GCompOpAssignInverted,
	GCompOpOr,
	GCompOpAnd,
	GCompOpClear,
	GCompOpSet
} GCompOp;


typedef struct GBitmap GBitmap;
typedef int GBitmapFormat; // no forward declarations of enums

// Framebuffer

struct n_GContext;

GBitmap * graphics_capture_frame_buffer(struct n_GContext * ctx);
GBitmap * graphics_capture_frame_buffer_format(struct n_GContext * ctx, GBitmapFormat format);
bool graphics_release_frame_buffer(struct n_GContext * ctx, GBitmap * bitmap);

#endif
