#pragma once
#include <pebble.h>

void radial_draw_init(void); 
void fill_outer_segment(GContext *ctx, int seg);
void fill_inner_segment(GContext *ctx, int seg);
void draw_radial_line(GContext *ctx, int seg);
void draw_inner_shape(GContext *ctx);
