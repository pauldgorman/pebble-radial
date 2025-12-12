/**
 * RADIAL TIME - Pebble 2 Watchface
 * Inner shape: octagon with horizontal top/bottom, vertical left/right
 */

#include <pebble.h>
#include "radial_geometry.h"
#include "radial_draw.h"

static Window *s_window;
static Layer *s_canvas_layer;
static int s_hour = 0;
static int s_minute = 0;

static void canvas_update_proc(Layer *layer, GContext *ctx) {
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
    
    int hour_segs = get_num_hour_segments(s_hour);
    int min_segs = get_num_min_segments(s_minute);
    
    graphics_context_set_fill_color(ctx, GColorWhite);
    for (int i = 0; i < hour_segs; i++) fill_outer_segment(ctx, i);
    for (int i = 0; i < min_segs; i++) fill_inner_segment(ctx, i);
    
    // Draw inner octagon shape first
    graphics_context_set_stroke_width(ctx, 4); // LINE_WIDTH hardcoded in draw.c as well, or expose it
    graphics_context_set_stroke_color(ctx, GColorBlack);
    draw_inner_shape(ctx);
    
    // Draw all 12 radial lines last so they appear on top
    for (int i = 0; i < 12; i++) {
        graphics_context_set_stroke_color(ctx, GColorBlack);
        draw_radial_line(ctx, i);
    }
}

// Debug mode: 1 = fast cycle, 0 = normal time
#define DEBUG_MODE 1

static void update_time(void *data) {
    if (DEBUG_MODE) {
        // In debug mode, we just increment the time manually
        s_minute++;
        if (s_minute >= 60) {
            s_minute = 0;
            s_hour++;
            if (s_hour >= 24) s_hour = 0;
        }
        // Schedule next update in 500ms (2x speed of 1 min/sec request, 1 sec is also fine)
        // User asked for "one minute every second" -> 1000ms
        app_timer_register(1000, update_time, NULL);
    } else {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        s_hour = t->tm_hour;
        s_minute = t->tm_min;
    }
    if (s_canvas_layer) layer_mark_dirty(s_canvas_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    if (!DEBUG_MODE) {
        update_time(NULL);
    }
}

static void window_load(Window *window) {
    Layer *root = window_get_root_layer(window);
    s_canvas_layer = layer_create(layer_get_bounds(root));
    layer_set_update_proc(s_canvas_layer, canvas_update_proc);
    layer_add_child(root, s_canvas_layer);
    
    // In debug mode, we need a faster timer than the minute tick
    if (DEBUG_MODE) {
        // Initialize with a starting time
        s_hour = 12;
        s_minute = 0;
        update_time(NULL); // Initial call to start the loop
    } else {
        update_time(NULL);
    }
}

static void window_unload(Window *window) {
    layer_destroy(s_canvas_layer);
}

static void init(void) {
    geometry_init();
    radial_draw_init();
    
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers){
        .load = window_load, .unload = window_unload
    });
    window_stack_push(s_window, true);
    if (!DEBUG_MODE) {
        tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    }
}

static void deinit(void) {
    tick_timer_service_unsubscribe();
    window_destroy(s_window);
}

int main(void) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Radial Time Starting...");
    init();
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Init complete, entering loop");
    app_event_loop();
    deinit();
}
