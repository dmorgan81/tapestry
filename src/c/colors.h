#pragma once
#include <pebble.h>
#include <enamel.h>

static inline GColor colors_get_background_color(void) {
#ifdef PBL_COLOR
    return enamel_get_COLOR_BACKGROUND();
#else
    return enamel_get_COLOR_INVERT() ? GColorWhite : GColorBlack;
#endif
}

static inline GColor colors_get_foreground_color(void) {
#ifdef PBL_COLOR
    return enamel_get_COLOR_FOREGROUND();
#else
    return enamel_get_COLOR_INVERT() ? GColorBlack : GColorWhite;
#endif
}

static inline GColor colors_get_text_color(void) {
    return gcolor_legible_over(colors_get_foreground_color());
}
