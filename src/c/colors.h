#pragma once
#include <pebble.h>

static inline GColor colors_get_background_color(void) {
    return GColorBlack;
}

static inline GColor colors_get_foreground_color(void) {
    return PBL_IF_COLOR_ELSE(GColorRed, GColorWhite);
}

static inline GColor colors_get_text_color(void) {
    return gcolor_legible_over(colors_get_foreground_color());
}
