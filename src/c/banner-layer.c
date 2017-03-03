#include <pebble.h>
#include "logging.h"
#include "colors.h"
#include "banner-layer.h"

static void prv_update_proc(Layer *this, GContext *ctx) {
    log_func();
    GRect bounds = layer_get_bounds(this);

    uint8_t w = bounds.size.w / 3;
    uint8_t h = bounds.size.h / 2;

    graphics_context_set_stroke_color(ctx, colors_get_background_color());
    graphics_context_set_fill_color(ctx, colors_get_foreground_color());

    GRect rect = GRect(0, bounds.size.h - h, w, h);
    graphics_fill_rect(ctx, rect, 0, GCornerNone);

    rect = GRect(bounds.size.w - w, bounds.size.h - h, w, h);
    graphics_fill_rect(ctx, rect, 0, GCornerNone);

    uint8_t h2 = h / 2;
    rect = GRect(w / 2, 0, bounds.size.w - w, bounds.size.h - h2);
    graphics_fill_rect(ctx, rect, 0, GCornerNone);
    graphics_draw_rect(ctx, rect);

    graphics_draw_line(ctx, GPoint(w / 2, bounds.size.h - h2), GPoint(w, bounds.size.h));
    graphics_draw_line(ctx, GPoint(bounds.size.w - w, bounds.size.h), GPoint(bounds.size.w - (w / 2) - 2, bounds.size.h - h2));

    graphics_context_set_fill_color(ctx, colors_get_background_color());
    GPathInfo gpath_info = {
        .num_points = 3,
        .points = (GPoint[]) {
            {0, 0},
            {w / 3, h2},
            {0, h}
        }
    };
    GPath *gpath = gpath_create(&gpath_info);
    gpath_move_to(gpath, GPoint(-1, bounds.size.h - h));
    gpath_draw_filled(ctx, gpath);

    gpath_move_to(gpath, GPoint(bounds.size.w, bounds.size.h));
    gpath_rotate_to(gpath, DEG_TO_TRIGANGLE(180));
    gpath_draw_filled(ctx, gpath);

    gpath_destroy(gpath);
}

BannerLayer *banner_layer_create(GRect frame) {
    BannerLayer *this = layer_create(frame);
    layer_set_update_proc(this, prv_update_proc);
    return this;
}

void banner_layer_destroy(BannerLayer *this) {
    log_func();
    layer_destroy(this);
}
