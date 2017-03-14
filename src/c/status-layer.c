#include <pebble.h>
#include <pebble-events/pebble-events.h>
#include "logging.h"
#include "colors.h"
#include "status-layer.h"

typedef struct {
    bool connected;
    EventHandle connection_event_handle;
} Data;

static void prv_update_proc(Layer *this, GContext *ctx) {
    log_func();
    Data *data = layer_get_data(this);
    GRect frame = layer_get_frame(this);

    if (quiet_time_is_active()) {
        GPathInfo gpath_info = {
            .num_points = 6,
            .points = (GPoint[]) {
                {15, 0},
                {15, 18},
                {5, 12},
                {0, 12},
                {0, 7},
                {4, 7}
            }
        };

        GPath *gpath = gpath_create(&gpath_info);
        gpath_move_to(gpath, GPoint(frame.origin.x - 3, frame.origin.y + 3));

        graphics_context_set_fill_color(ctx, colors_get_text_color());
        gpath_draw_filled(ctx, gpath);

        graphics_context_set_stroke_color(ctx, colors_get_text_color());
        graphics_draw_line(ctx, GPoint(5, 6), GPoint(23, 20));
        graphics_context_set_stroke_width(ctx, 2);
        graphics_context_set_stroke_color(ctx, colors_get_foreground_color());
        graphics_draw_line(ctx, GPoint(10, 10), GPoint(18, 16));

        gpath_destroy(gpath);
    } else if (data->connected) {
        GPathInfo gpath_info = {
            .num_points = 7,
            .points = (GPoint []) {
                {0, 8},
                {8, 16},
                {4, 20},
                {4, 4},
                {8, 8},
                {0, 16},
                {4, 12}
            }
        };

        GPath *gpath = gpath_create(&gpath_info);
        gpath_move_to(gpath, frame.origin);

        graphics_context_set_stroke_color(ctx, colors_get_text_color());
        gpath_draw_outline(ctx, gpath);

        gpath_destroy(gpath);
    }
}

static void prv_connection_handler(bool connected, void *this) {
    log_func();
    Data *data = layer_get_data(this);
    data->connected = connected;
    layer_mark_dirty(this);
}

StatusLayer *status_layer_create(GRect frame) {
    log_func();
    StatusLayer *this = layer_create_with_data(frame, sizeof(Data));
    layer_set_update_proc(this, prv_update_proc);
    Data *data = layer_get_data(this);

    data->connected = connection_service_peek_pebble_app_connection();
    data->connection_event_handle = events_connection_service_subscribe_context((EventConnectionHandlers) {
        .pebble_app_connection_handler = prv_connection_handler
    }, this);

    return this;
}

void status_layer_destroy(StatusLayer *this) {
    log_func();
    Data *data = layer_get_data(this);
    events_connection_service_unsubscribe(data->connection_event_handle);
    layer_destroy(this);
}
