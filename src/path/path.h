#pragma once
#include <pebble.h>
#include "../context.h"
#include "../types.h"

#include "../primitives/line.h"

typedef struct {
    uint32_t num_points;
    n_GPoint * points;
} n_GPathInfo;

typedef struct {
    uint32_t num_points;
    n_GPoint * points;
    int32_t angle;
    n_GPoint offset;
    bool open;
} n_GPath;

n_GPath * n_gpath_create(n_GPathInfo * path_info);
void n_gpath_destroy(n_GPath * path);

void n_gpath_draw(n_GContext * ctx, n_GPath * path);
void n_gpath_draw_open(n_GContext * ctx, n_GPath * path);
void n_gpath_fill(n_GContext * ctx, n_GPath * path);

void n_gpath_rotate_to(n_GPath * path, int32_t angle);
void n_gpath_move_to(n_GPath * path, n_GPoint offset);
