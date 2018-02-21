#include "pebble.h"

#include <math.h>

static const double _2PI = 2 * 3.141592653;

int32_t sin_lookup(int32_t angle) {
    double fangle = ((double)angle) / TRIG_MAX_ANGLE * _2PI;
    return (int32_t)(sin(fangle) * TRIG_MAX_RATIO);
}

int32_t cos_lookup(int32_t angle) {
    double fangle = ((double)angle) / TRIG_MAX_ANGLE * _2PI;
    return (int32_t)(cos(fangle) * TRIG_MAX_RATIO);
}
