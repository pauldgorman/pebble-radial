/**
 * RADIAL TIME - Pebble 2 Watchface
 * Inner shape: octagon with horizontal top/bottom, vertical left/right
 */

#include <pebble.h>

// ===== CONFIGURABLE DIMENSIONS =====
#define W 144
#define H 168
#define CX (W/2)              // 72
#define CY (H/2)              // 84
#define LINE_WIDTH 4

// Inner size (configurable)
#define INNER_V (H * 40 / 100) // ~67 - vertical distance from center
#define INNER_H (W * 40 / 100) // ~58 - horizontal distance from center

// Inner rectangle bounds
#define INNER_TOP    (CY - INNER_V)   // 42
#define INNER_BOTTOM (CY + INNER_V)   // 126
#define INNER_LEFT   (CX - INNER_H)   // 36
#define INNER_RIGHT  (CX + INNER_H)   // 108

// Calculate where each radial line hits the octagon edge
// With INNER_V = 67 and INNER_H = 58:
// INNER_TOP = 17, INNER_BOTTOM = 151, INNER_LEFT = 14, INNER_RIGHT = 130

// Line 0: (CX, INNER_TOP) - straight up
#define P0_X CX
#define P0_Y INNER_TOP

// Line 1 goes from (72,84) to (120,0). Direction: (48, -84)
// At y=INNER_TOP (17): t = (84-17)/84 = 0.797619, x = 72 + 48*0.797619 = 110
#define P1_X 110
#define P1_Y INNER_TOP

// Line 2 goes from (72,84) to (144,35). Direction: (72, -49)
// Hits right edge at x=INNER_RIGHT (130): t = (130-72)/72 = 0.8056, y = 84 - 49*0.8056 = 45
#define P2_X INNER_RIGHT
#define P2_Y 45

// Line 3: (INNER_RIGHT, CY) - straight right
#define P3_X INNER_RIGHT
#define P3_Y CY

// Line 4 goes from (72,84) to (144,133). Direction: (72, 49)
// Hits right edge at x=INNER_RIGHT (130): t = 0.8056, y = 84 + 49*0.8056 = 123
#define P4_X INNER_RIGHT
#define P4_Y 123

// Line 5 goes from (72,84) to (120,168). Direction: (48, 84)
// At y=INNER_BOTTOM (151): t = (151-84)/84 = 0.797619, x = 72 + 48*0.797619 = 110
#define P5_X 110
#define P5_Y INNER_BOTTOM

// Line 6: (CX, INNER_BOTTOM) - straight down
#define P6_X CX
#define P6_Y INNER_BOTTOM

// Line 7 goes from (72,84) to (24,168). Direction: (-48, 84)
// At y=INNER_BOTTOM (151): t = 0.797619, x = 72 - 48*0.797619 = 34
#define P7_X 34
#define P7_Y INNER_BOTTOM

// Line 8 goes from (72,84) to (0,133). Direction: (-72, 49)
// Hits left edge at x=INNER_LEFT (14): t = (72-14)/72 = 0.8056, y = 84 + 49*0.8056 = 123
#define P8_X INNER_LEFT
#define P8_Y 123

// Line 9: (INNER_LEFT, CY) - straight left
#define P9_X INNER_LEFT
#define P9_Y CY

// Line 10 goes from (72,84) to (0,35). Direction: (-72, -49)
// Hits left edge at x=INNER_LEFT (14): t = 0.8056, y = 84 - 49*0.8056 = 45
#define P10_X INNER_LEFT
#define P10_Y 45

// Line 11 goes from (72,84) to (24,0). Direction: (-48, -84)
// At y=INNER_TOP (17): t = 0.797619, x = 72 - 48*0.797619 = 34
#define P11_X 34
#define P11_Y INNER_TOP

static Window *s_window;
static Layer *s_canvas_layer;
static int s_hour = 0;
static int s_minute = 0;

// Screen edge points
static const GPoint EDGE[] = {
    {72, 0}, {120, 0}, {144, 35}, {144, 84},
    {144, 133}, {120, 168}, {72, 168}, {24, 168},
    {0, 133}, {0, 84}, {0, 35}, {24, 0}
};

// Screen corners
static const GPoint CORNERS[] = {{W, 0}, {W, H}, {0, H}, {0, 0}};
static const int SEG_CORNER[] = {-1, 0, -1, -1, 1, -1, -1, 2, -1, -1, 3, -1};

// Inner points (12 points where radial lines meet inner shape)
static const GPoint INNER[] = {
    {P0_X, P0_Y},   // 0
    {P1_X, P1_Y},   // 1
    {P2_X, P2_Y},   // 2
    {P3_X, P3_Y},   // 3
    {P4_X, P4_Y},   // 4
    {P5_X, P5_Y},   // 5
    {P6_X, P6_Y},   // 6
    {P7_X, P7_Y},   // 7
    {P8_X, P8_Y},   // 8
    {P9_X, P9_Y},   // 9
    {P10_X, P10_Y}, // 10
    {P11_X, P11_Y}, // 11
};

static void fill_outer_segment(GContext *ctx, int seg) {
    GPoint p1 = EDGE[seg];
    GPoint p2 = EDGE[(seg + 1) % 12];
    GPoint i1 = INNER[seg];
    GPoint i2 = INNER[(seg + 1) % 12];
    int corner = SEG_CORNER[seg];
    
    GPoint pts[5];
    int n = 0;
    pts[n++] = i1;
    pts[n++] = p1;
    if (corner != -1) pts[n++] = CORNERS[corner];
    pts[n++] = p2;
    pts[n++] = i2;
    
    GPathInfo info = {.num_points = n, .points = pts};
    GPath *path = gpath_create(&info);
    gpath_draw_filled(ctx, path);
    gpath_destroy(path);
}

static void fill_inner_segment(GContext *ctx, int seg) {
    GPoint center = GPoint(CX, CY);
    GPoint p1 = INNER[seg];
    GPoint p2 = INNER[(seg + 1) % 12];
    
    GPoint pts[3] = {center, p1, p2};
    GPathInfo info = {.num_points = 3, .points = pts};
    GPath *path = gpath_create(&info);
    gpath_draw_filled(ctx, path);
    gpath_destroy(path);
}

static void draw_radial_line(GContext *ctx, int seg) {
    graphics_draw_line(ctx, GPoint(CX, CY), EDGE[seg]);
}

static void draw_inner_shape(GContext *ctx) {
    // Top edge (horizontal): 11 -> 0 -> 1
    graphics_draw_line(ctx, INNER[11], INNER[1]);
    // Right edge (vertical): 2 -> 3 -> 4
    graphics_draw_line(ctx, INNER[2], INNER[4]);
    // Bottom edge (horizontal): 5 -> 6 -> 7
    graphics_draw_line(ctx, INNER[7], INNER[5]);
    // Left edge (vertical): 8 -> 9 -> 10
    graphics_draw_line(ctx, INNER[10], INNER[8]);
    
    // Corner connectors (diagonal)
    graphics_draw_line(ctx, INNER[1], INNER[2]);   // top-right
    graphics_draw_line(ctx, INNER[4], INNER[5]);   // bottom-right
    graphics_draw_line(ctx, INNER[7], INNER[8]);   // bottom-left
    graphics_draw_line(ctx, INNER[10], INNER[11]); // top-left
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
    
    int hour_segs = s_hour % 12;
    if (hour_segs == 0) hour_segs = 12;
    int min_segs = s_minute / 5;
    
    graphics_context_set_fill_color(ctx, GColorWhite);
    for (int i = 0; i < hour_segs; i++) fill_outer_segment(ctx, i);
    for (int i = 0; i < min_segs; i++) fill_inner_segment(ctx, i);
    
    // Draw inner octagon shape first
    graphics_context_set_stroke_width(ctx, LINE_WIDTH);
    graphics_context_set_stroke_color(ctx, GColorBlack);
    draw_inner_shape(ctx);
    
    // Draw all 12 radial lines last so they appear on top
    for (int i = 0; i < 12; i++) {
        graphics_context_set_stroke_color(ctx, GColorBlack);
        draw_radial_line(ctx, i);
    }
}

static void update_time() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    s_hour = t->tm_hour;
    s_minute = t->tm_min;
    if (s_canvas_layer) layer_mark_dirty(s_canvas_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static void window_load(Window *window) {
    Layer *root = window_get_root_layer(window);
    s_canvas_layer = layer_create(layer_get_bounds(root));
    layer_set_update_proc(s_canvas_layer, canvas_update_proc);
    layer_add_child(root, s_canvas_layer);
    update_time();
}

static void window_unload(Window *window) {
    layer_destroy(s_canvas_layer);
}

static void init(void) {
    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers){
        .load = window_load, .unload = window_unload
    });
    window_stack_push(s_window, true);
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(void) {
    tick_timer_service_unsubscribe();
    window_destroy(s_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
