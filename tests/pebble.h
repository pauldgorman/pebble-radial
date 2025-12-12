#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Mock basic types
typedef struct GPoint {
    int16_t x;
    int16_t y;
} GPoint;

typedef struct GRect {
    GPoint origin;
    struct {
        int16_t w;
        int16_t h;
    } size;
} GRect;

// Helper to create points inline
static inline GPoint MakeGPoint(int x, int y) {
    return (GPoint){(int16_t)x, (int16_t)y};
}

#define GPoint(x, y) MakeGPoint(x, y)
