#include <pebble.h>
#include <pebble-events/pebble-events.h>
#include "logging.h"
#include "colors.h"
#include "tapestry-layer.h"
#include "time-layer.h"
#include "banner-layer.h"
#include "date-layer.h"

static Window *s_window;
static TapestryLayer *s_tapestry_layer;
static TimeLayer *s_time_layer;
static BannerLayer *s_banner_layer;
static DateLayer *s_date_layer;

static EventHandle s_accel_tap_event_handle;
static AppTimer *s_app_timer;

static void prv_app_timer_callback(void *context) {
    log_func();
    s_app_timer = NULL;

    GRect from = layer_get_frame(s_tapestry_layer);
    uint8_t h = from.size.h / 3;
    GRect to = GRect(from.origin.x, from.origin.y - h, from.size.w, from.size.h);
    PropertyAnimation *animation = property_animation_create_layer_frame(s_tapestry_layer, &from, &to);
    animation_schedule(property_animation_get_animation(animation));
}

static void prv_accel_tap_handler(AccelAxisType axis, int32_t direction) {
    log_func();
    if (s_app_timer) return;
    s_app_timer = app_timer_register(5000, prv_app_timer_callback, NULL);

    GRect from = layer_get_frame(s_tapestry_layer);
    uint8_t h = from.size.h / 3;
    GRect to = GRect(from.origin.x, from.origin.y + h, from.size.w, from.size.h);
    PropertyAnimation *animation = property_animation_create_layer_frame(s_tapestry_layer, &from, &to);
    animation_schedule(property_animation_get_animation(animation));
}

static void prv_window_load(Window *window) {
    log_func();
    Layer *root_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root_layer);
    uint8_t h = bounds.size.h / 3;

    s_tapestry_layer = tapestry_layer_create(GRect(PBL_IF_RECT_ELSE(30, 50), 0, bounds.size.w - PBL_IF_RECT_ELSE(60, 100), bounds.size.h));
    GRect frame = layer_get_frame(s_tapestry_layer);
    frame.origin.y -= h;
    layer_set_frame(s_tapestry_layer, frame);

    GRect rect = layer_get_bounds(s_tapestry_layer);
    h = rect.size.h / 3;
    s_time_layer = time_layer_create(GRect(5, h + PBL_IF_RECT_ELSE(4, 10), rect.size.w - 10, rect.size.h - h - PBL_IF_RECT_ELSE(7, 17)), s_tapestry_layer);

    h = bounds.size.h / 4;
    s_banner_layer = banner_layer_create(GRect(PBL_IF_RECT_ELSE(5, 30), bounds.size.h - h, bounds.size.w - PBL_IF_RECT_ELSE(10, 60), h - 10));
#ifdef PBL_ROUND
    frame = layer_get_frame(s_banner_layer);
    frame.origin.y -= 10;
    layer_set_frame(s_banner_layer, frame);
#endif

    bounds = layer_get_bounds(s_banner_layer);
    s_date_layer = date_layer_create(GRect(0, 12, bounds.size.w, bounds.size.h - 24), s_banner_layer);

    layer_add_child(root_layer, s_banner_layer);
    layer_add_child(root_layer, s_tapestry_layer);

    window_set_background_color(window, colors_get_background_color());

    s_accel_tap_event_handle = events_accel_tap_service_subscribe(prv_accel_tap_handler);
}

static void prv_window_unload(Window *window) {
    log_func();
    date_layer_destroy(s_date_layer);
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
