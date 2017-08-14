#include "rect.h"

n_GPoint n_grect_center_point(n_GRect *rect) {
    int16_t x, y;
    x = (rect->size.w - rect->origin.x) / 2;
    y = (rect->size.h - rect->origin.y) / 2;
    return n_GPoint(x, y);
}
