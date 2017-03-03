#include <pebble.h>
#include "logging.h"

static Window *s_window;

static void prv_window_load(Window *window) {
    log_func();
}

static void prv_window_unload(Window *window) {
    log_func();
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
