#pragma once
#include <pebble.h>
#include "../common.h"
#include "circle.h"
#include "line.h"

void n_graphics_draw_rect(n_GContext * ctx, n_GRect rect, uint16_t radius, n_GCornerMask mask);
void n_graphics_fill_rect(n_GContext * ctx, n_GRect rect, uint16_t radius, n_GCornerMask mask);

// DEPRECATED
void n_graphics_draw_thin_rect(n_GContext * ctx, n_GRect rect);
