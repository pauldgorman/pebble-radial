#include "radial_geometry.h"

// Inner rectangle bounds
#define INNER_TOP    (CY - INNER_V)
#define INNER_BOTTOM (CY + INNER_V)
#define INNER_LEFT   (CX - INNER_H)
#define INNER_RIGHT  (CX + INNER_H)

// Calculated points
#define P0_X CX
#define P0_Y INNER_TOP
#define P1_X 110
#define P1_Y INNER_TOP
#define P2_X INNER_RIGHT
#define P2_Y 45
#define P3_X INNER_RIGHT
#define P3_Y CY
#define P4_X INNER_RIGHT
#define P4_Y 123
#define P5_X 110
#define P5_Y INNER_BOTTOM
#define P6_X CX
#define P6_Y INNER_BOTTOM
#define P7_X 34
#define P7_Y INNER_BOTTOM
#define P8_X INNER_LEFT
#define P8_Y 123
#define P9_X INNER_LEFT
#define P9_Y CY
#define P10_X INNER_LEFT
#define P10_Y 45
#define P11_X 34
#define P11_Y INNER_TOP

// Screen edge points
static const GPoint EDGE[] = {
    {72, 0}, {120, 0}, {144, 35}, {144, 84},
    {144, 133}, {120, 168}, {72, 168}, {24, 168},
    {0, 133}, {0, 84}, {0, 35}, {24, 0}
};

// Screen corners
static const GPoint CORNERS[] = {{W, 0}, {W, H}, {0, H}, {0, 0}};
static const int SEG_CORNER[] = {-1, 0, -1, -1, 1, -1, -1, 2, -1, -1, 3, -1};

// Inner points
static const GPoint INNER[] = {
    {P0_X, P0_Y}, {P1_X, P1_Y}, {P2_X, P2_Y}, {P3_X, P3_Y},
    {P4_X, P4_Y}, {P5_X, P5_Y}, {P6_X, P6_Y}, {P7_X, P7_Y},
    {P8_X, P8_Y}, {P9_X, P9_Y}, {P10_X, P10_Y}, {P11_X, P11_Y}
};

void geometry_init(void) {
    // Any runtime initialization if needed (currently all static const)
}

GPoint get_edge_point(int index) {
    return EDGE[index % 12];
}

GPoint get_inner_point(int index) {
    return INNER[index % 12];
}

GPoint get_corner_point(int index) {
    if (index >= 0 && index < 4) {
        return CORNERS[index];
    }
    return GPoint(0,0);
}

int get_seg_corner_index(int seg_index) {
    return SEG_CORNER[seg_index % 12];
}

int get_num_hour_segments(int hour) {
    int segs = hour % 12;
    if (segs == 0) segs = 12;
    return segs;
}

int get_num_min_segments(int minute) {
    return minute / 5;
}
