#include "rect.h"
#include <stddef.h>

n_GPoint n_grect_center_point(n_GRect *rect) {
    int16_t x, y;
    x = rect->origin.x + rect->size.w / 2;
    y = rect->origin.y + rect->size.h / 2;
    return n_GPoint(x, y);
}

bool n_gpoint_equal(const n_GPoint *point_a, const n_GPoint *point_b) {
    if (point_a == NULL || point_b == NULL)
        return false;
    return
        point_a->x == point_b->x &&
        point_a->y == point_b->y;
}

bool n_gsize_equal(const n_GSize *size_a, const n_GSize *size_b) {
    if (size_a == NULL || size_b == NULL)
        return false;
    return
        size_a->w == size_b->w &&
        size_a->h == size_b->h;
}

bool n_grect_equal(const n_GRect *rect_a, const n_GRect *rect_b) {
    if (rect_a == NULL || rect_b == NULL)
        return false;
    return
        rect_a->origin.x == rect_b->origin.x &&
        rect_a->origin.y == rect_b->origin.y &&
        rect_a->size.w == rect_b->size.w &&
        rect_a->size.h == rect_b->size.h;
}

bool n_grect_is_empty(const n_GRect *const rect) {
    return rect == NULL || (rect->size.w == 0 && rect->size.h == 0);
}

void n_grect_clip(n_GRect *rect_to_clip, const n_GRect* rect_clipper) {
    if (rect_to_clip == NULL || rect_clipper == NULL)
        return;
    if (rect_to_clip->origin.x < rect_clipper->origin.x) {
        rect_to_clip->size.w -= rect_clipper->origin.x - rect_to_clip->origin.x;
        rect_to_clip->origin.x = rect_clipper->origin.x;
    }
    if (rect_to_clip->origin.y < rect_clipper->origin.y) {
        rect_to_clip->size.h -= rect_clipper->origin.y - rect_to_clip->origin.y;
        rect_to_clip->origin.y = rect_clipper->origin.y;
    }

    int16_t clipper_right = rect_clipper->origin.x + rect_clipper->size.w;
    if (rect_to_clip->origin.x + rect_to_clip->size.w > clipper_right) {
        rect_to_clip->size.w = clipper_right - rect_to_clip->origin.x;
    }
    int16_t clipper_bottom = rect_clipper->origin.y + rect_clipper->size.h;
    if (rect_to_clip->origin.y + rect_to_clip->size.h > clipper_bottom) {
        rect_to_clip->size.h = clipper_bottom - rect_to_clip->origin.y;
    }

    if (rect_to_clip->size.w < 0) {
        rect_to_clip->size.w = 0;
    }
    if (rect_to_clip->size.h < 0) {
        rect_to_clip->size.h = 0;
    }
}

bool n_grect_contains_point(const n_GRect *rect, const n_GPoint *point) {
    if (rect == NULL || point == NULL)
        return false;
    return
        point->x >= rect->origin.x &&
        point->y >= rect->origin.y &&
        point->x < rect->origin.x + rect->size.w &&
        point->y < rect->origin.y + rect->size.h;
}

n_GRect n_grect_crop(n_GRect rect, int32_t crop_size_px) {
    return n_GRect(
        rect.origin.x + crop_size_px / 2,
        rect.origin.y + crop_size_px / 2,
        rect.size.w < crop_size_px ? 0 : rect.size.w - crop_size_px,
        rect.size.h < crop_size_px ? 0 : rect.size.h - crop_size_px
    );
}
