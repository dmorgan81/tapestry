#include <pebble.h>
#include "logging.h"
#include "colors.h"
#include "tapestry-layer.h"

static const GPoint BORDER_WIDTH = { .x = 3, .y = 3 };

static void prv_update_proc(Layer *this, GContext *ctx) {
    log_func();
    GRect bounds = layer_get_bounds(this);

    GPathInfo gpath_info = {
        .num_points = 5,
        .points = (GPoint[]) {
            {0, 0},
            {0, bounds.size.h},
            {bounds.size.w / 2, bounds.size.h - (bounds.size.h / 16)},
            {bounds.size.w, bounds.size.h},
            {bounds.size.w, 0}
        }
    };
    GPath *gpath = gpath_create(&gpath_info);
    graphics_context_set_fill_color(ctx, colors_get_foreground_color());
    gpath_draw_filled(ctx, gpath);
    gpath_destroy(gpath);

    gpath_info.points[0].x += BORDER_WIDTH.x;
    gpath_info.points[1].x += BORDER_WIDTH.x;
    gpath_info.points[1].y -= BORDER_WIDTH.y;
    gpath_info.points[2].y -= BORDER_WIDTH.y;
    gpath_info.points[3].x -= BORDER_WIDTH.x;
    gpath_info.points[3].y -= BORDER_WIDTH.y;
    gpath_info.points[4].x -= BORDER_WIDTH.x;

    gpath = gpath_create(&gpath_info);
    graphics_context_set_stroke_color(ctx, colors_get_background_color());
    gpath_draw_outline_open(ctx, gpath);
    gpath_destroy(gpath);
}

TapestryLayer *tapestry_layer_create(GRect frame) {
    log_func();
    TapestryLayer *this = layer_create(frame);
    layer_set_update_proc(this, prv_update_proc);
    return this;
}

void tapestry_layer_destroy(TapestryLayer *this) {
    log_func();
    layer_destroy(this);
}
