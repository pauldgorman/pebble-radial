#pragma once
#include <pebble.h>

// Dimensions
#define W 144
#define H 168
#define CX (W/2)
#define CY (H/2)

// Inner size
#define INNER_V (H * 40 / 100)
#define INNER_H (W * 40 / 100)

// Function prototypes
void geometry_init(void);
GPoint get_edge_point(int index);
GPoint get_inner_point(int index);
GPoint get_corner_point(int index);
int get_seg_corner_index(int seg_index);
int get_num_hour_segments(int hour);
int get_num_min_segments(int minute);
