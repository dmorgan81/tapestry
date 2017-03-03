#pragma once
#include <pebble.h>

static inline GColor colors_get_background_color(void) {
    return GColorBlack;
}

static inline GColor colors_get_tapestry_color(void) {
    return PBL_IF_COLOR_ELSE(GColorRed, GColorWhite);
}
