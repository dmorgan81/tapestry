#include <pebble.h>
#include <pebble-events/pebble-events.h>
#include <pebble-fctx/fctx.h>
#include <pebble-fctx/ffont.h>
#include <ctype.h>
#include "logging.h"
#include "colors.h"
#include "date-layer.h"

typedef struct {
    Layer *parent_layer;
    FFont *font;
    struct tm tick_time;
    EventHandle tick_timer_event_handle;
} Data;

static inline void strupp(char *s) {
    while ((*s++ = (char) toupper((int) *s)));
}

static void prv_update_proc(Layer *this, GContext *ctx) {
    log_func();
    Data *data = layer_get_data(this);
    GRect frame = layer_get_frame(this);
    GRect parent_frame = layer_get_frame(data->parent_layer);
    FPoint origin = g2fpoint(parent_frame.origin);

    char s[16];
    strftime(s, sizeof(s), PBL_IF_RECT_ELSE("%a %b %d", "%b %d"), &data->tick_time);
    strupp(s);

    FContext fctx;
    fctx_init_context(&fctx, ctx);
    fctx_set_offset(&fctx, fpoint_add(origin, FPointI(frame.origin.x + (frame.size.w / 2), frame.origin.y)));
    fctx_set_fill_color(&fctx, colors_get_text_color());

    int16_t font_size = PBL_IF_RECT_ELSE(14, 16);
    fixed_t str_width;
    GRect rect = grect_inset(frame, GEdgeInsets(0, 2));
    fixed_t width = INT_TO_FIXED(rect.size.w);

    do {
            fctx_set_text_em_height(&fctx, data->font, font_size--);
            str_width = fctx_string_width(&fctx, s, data->font);
        } while (str_width > width);

    fctx_set_text_em_height(&fctx, data->font, ++font_size);

    fctx_begin_fill(&fctx);
    fctx_draw_string(&fctx, s, data->font, GTextAlignmentCenter, FTextAnchorMiddle);
    fctx_end_fill(&fctx);

    fctx_deinit_context(&fctx);
}

static void prv_tick_handler(struct tm *tick_time, TimeUnits units_changed, void *this) {
    log_func();
    Data *data = layer_get_data(this);
    memcpy(&data->tick_time, tick_time, sizeof(struct tm));
    layer_mark_dirty(this);
}

DateLayer *date_layer_create(GRect frame, Layer *parent_layer) {
    log_func();
    DateLayer *this = layer_create_with_data(frame, sizeof(Data));
    layer_set_update_proc(this, prv_update_proc);
    Data *data = layer_get_data(this);

    data->parent_layer = parent_layer;
    data->font = ffont_create_from_resource(RESOURCE_ID_LECO_FFONT);

    time_t now = time(NULL);
    prv_tick_handler(localtime(&now), DAY_UNIT, this);
    data->tick_timer_event_handle = events_tick_timer_service_subscribe_context(DAY_UNIT, prv_tick_handler, this);

    layer_add_child(parent_layer, this);

    return this;
}

void date_layer_destroy(DateLayer *this) {
    log_func();
    Data *data = layer_get_data(this);
    events_tick_timer_service_unsubscribe(data->tick_timer_event_handle);
    ffont_destroy(data->font);
    layer_destroy(this);
}
