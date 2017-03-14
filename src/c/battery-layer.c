#include <pebble.h>
#include <pebble-events/pebble-events.h>
#include "logging.h"
#include "colors.h"
#include "battery-layer.h"

typedef struct {
    BatteryChargeState state;
    EventHandle battery_state_event_handle;
} Data;

static void prv_update_proc(Layer *this, GContext *ctx) {
    log_func();
    Data *data = layer_get_data(this);

    char s[5];
    snprintf(s, sizeof(s), "%d%%", data->state.charge_percent);

    graphics_context_set_text_color(ctx, colors_get_text_color());
    graphics_draw_text(ctx, s, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21), layer_get_bounds(this),
        GTextOverflowModeFill, GTextAlignmentRight, NULL);
}

static void prv_battery_state_handler(BatteryChargeState state, void *this) {
    log_func();
    Data *data = layer_get_data(this);
    data->state = state;
    layer_mark_dirty(this);
}

BatteryLayer *battery_layer_create(GRect frame) {
    log_func();
    BatteryLayer *this = layer_create_with_data(frame, sizeof(Data));
    layer_set_update_proc(this, prv_update_proc);
    Data *data = layer_get_data(this);

    data->state = battery_state_service_peek();
    data->battery_state_event_handle = events_battery_state_service_subscribe_context(prv_battery_state_handler, this);

    return this;
}

void battery_layer_destroy(BatteryLayer *this) {
    log_func();
    Data *data = layer_get_data(this);
    events_battery_state_service_unsubscribe(data->battery_state_event_handle);
    layer_destroy(this);
}
