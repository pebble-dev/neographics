#pragma once
#include <pebble.h>
#include "../common.h"
#include "circle.h"

void n_graphics_prv_draw_1px_line_bounded(
    n_GContext * ctx, n_GPoint from, n_GPoint to,
    int16_t minx, int16_t maxx, int16_t miny, int16_t maxy);
void n_graphics_prv_draw_thick_line_bounded(
    n_GContext * ctx, n_GPoint from, n_GPoint to, uint8_t width,
    int16_t minx, int16_t maxx, int16_t miny, int16_t maxy);
void n_graphics_draw_line(n_GContext * ctx, n_GPoint from, n_GPoint to);
