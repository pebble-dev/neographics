#include "rect.h"

n_GPoint n_grect_center_point(n_GRect *rect) {
    int16_t x, y;
    x = rect->origin.x + rect->size.w / 2;
    y = rect->origin.y + rect->size.h / 2;
    return n_GPoint(x, y);
}
