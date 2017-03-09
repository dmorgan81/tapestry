#include <pebble.h>
#include "logging.h"
#include "colors.h"
#include "tapestry-layer.h"
#include "time-layer.h"
#include "banner-layer.h"

static Window *s_window;
static TapestryLayer *s_tapestry_layer;
static TimeLayer *s_time_layer;
static BannerLayer *s_banner_layer;

static void prv_window_load(Window *window) {
    log_func();
    Layer *root_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root_layer);
    uint8_t h = bounds.size.h / 3;

    s_tapestry_layer = tapestry_layer_create(GRect(PBL_IF_RECT_ELSE(30, 50), 0, bounds.size.w - PBL_IF_RECT_ELSE(60, 100), bounds.size.h - h));
    layer_add_child(root_layer, s_tapestry_layer);

    GRect frame = layer_get_frame(s_tapestry_layer);
    s_time_layer = time_layer_create(GRect(frame.origin.x + 5, frame.origin.y + PBL_IF_RECT_ELSE(4, 8), frame.size.w - 10, frame.size.h - PBL_IF_RECT_ELSE(7, 11)));
    layer_add_child(s_tapestry_layer, s_time_layer);

    h = bounds.size.h / 4;
    s_banner_layer = banner_layer_create(GRect(PBL_IF_RECT_ELSE(5, 30), bounds.size.h - h, bounds.size.w - PBL_IF_RECT_ELSE(10, 60), h - 10));
#ifdef PBL_ROUND
    frame = layer_get_frame(s_banner_layer);
    frame.origin.y -= 10;
    layer_set_frame(s_banner_layer, frame);
#endif
    layer_add_child(root_layer, s_banner_layer);

    window_set_background_color(window, colors_get_background_color());
}

static void prv_window_unload(Window *window) {
    log_func();
    banner_layer_destroy(s_banner_layer);
    time_layer_destroy(s_time_layer);
    tapestry_layer_destroy(s_tapestry_layer);
}

static void prv_init(void) {
    log_func();
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .load = prv_window_load,
        .unload = prv_window_unload
    });
    window_stack_push(s_window, true);
}

static void prv_deinit(void) {
    log_func();
    window_destroy(s_window);
}

int main(void) {
    log_func();
    prv_init();
    app_event_loop();
    prv_deinit();
}
