#include "radial_draw.h"
#include "radial_geometry.h"

#define LINE_WIDTH 4

void radial_draw_init(void) {
    // Initialization for drawing if needed
}

void fill_outer_segment(GContext *ctx, int seg) {
    GPoint p1 = get_edge_point(seg);
    GPoint p2 = get_edge_point(seg + 1);
    GPoint i1 = get_inner_point(seg);
    GPoint i2 = get_inner_point(seg + 1);
    int corner = get_seg_corner_index(seg);
    
    GPoint pts[5];
    int n = 0;
    pts[n++] = i1;
    pts[n++] = p1;
    if (corner != -1) pts[n++] = get_corner_point(corner);
    pts[n++] = p2;
    pts[n++] = i2;
    
    GPathInfo info = {.num_points = n, .points = pts};
    GPath *path = gpath_create(&info);
    gpath_draw_filled(ctx, path);
    gpath_destroy(path);
}

void fill_inner_segment(GContext *ctx, int seg) {
    GPoint center = GPoint(CX, CY);
    GPoint p1 = get_inner_point(seg);
    GPoint p2 = get_inner_point(seg + 1);
    
    GPoint pts[3] = {center, p1, p2};
    GPathInfo info = {.num_points = 3, .points = pts};
    GPath *path = gpath_create(&info);
    gpath_draw_filled(ctx, path);
    gpath_destroy(path);
}

void draw_radial_line(GContext *ctx, int seg) {
    graphics_draw_line(ctx, GPoint(CX, CY), get_edge_point(seg));
}

void draw_inner_shape(GContext *ctx) {
    // Top edge (horizontal): 11 -> 0 -> 1
    graphics_draw_line(ctx, get_inner_point(11), get_inner_point(1));
    // Right edge (vertical): 2 -> 3 -> 4
    graphics_draw_line(ctx, get_inner_point(2), get_inner_point(4));
    // Bottom edge (horizontal): 5 -> 6 -> 7
    graphics_draw_line(ctx, get_inner_point(7), get_inner_point(5));
    // Left edge (vertical): 8 -> 9 -> 10
    graphics_draw_line(ctx, get_inner_point(10), get_inner_point(8));
    
    // Corner connectors (diagonal)
    graphics_draw_line(ctx, get_inner_point(1), get_inner_point(2));   // top-right
    graphics_draw_line(ctx, get_inner_point(4), get_inner_point(5));   // bottom-right
    graphics_draw_line(ctx, get_inner_point(7), get_inner_point(8));   // bottom-left
    graphics_draw_line(ctx, get_inner_point(10), get_inner_point(11)); // top-left
}
