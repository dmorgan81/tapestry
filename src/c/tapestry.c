#include <pebble.h>
#include "logging.h"
#include "colors.h"
#include "tapestry-layer.h"

static Window *s_window;
static TapestryLayer *s_tapestry_layer;

static void prv_window_load(Window *window) {
    log_func();
    Layer *root_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root_layer);

    s_tapestry_layer = tapestry_layer_create(GRect(10, 0, bounds.size.w - 20, bounds.size.h - (bounds.size.h / 3)));
    layer_add_child(root_layer, s_tapestry_layer);

    window_set_background_color(window, colors_get_background_color());
}

static void prv_window_unload(Window *window) {
    log_func();
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
