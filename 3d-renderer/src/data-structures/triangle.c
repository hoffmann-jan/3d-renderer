#include "triangle.h"
#include <stdlib.h>
#include "../app/display.h"

void integer_swap(int *a, int *b) {
    int temporary_value = *a;
    *a = *b;
    *b = temporary_value;
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // Find the two slopes (two triangle legs)
    float inverse_slope_1 = (float)(x1 - x0) / (y1 - y0);
    float inverse_slope_2 = (float)(x2 - x0) / (y2 - y0);

    float x_start = x0;
    float x_end = x0;

    // Loop all scanlines from top to bottom
    for (int y = y0; y <= y2; y++) {
        draw_line(x_start, y, x_end, y, color);
        x_start += inverse_slope_1;
        x_end += inverse_slope_2;
    }
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // Find the two slopes (two triangle legs)
    float inverse_slope_1 = (float)(x2 - x0) / (y2 - y0);
    float inverse_slope_2 = (float)(x2 - x1) / (y2 - y1);

    float x_start = x2;
    float x_end = x2;

    // Loop all scanlines from top to bottom
    for (int y = y2; y >= y0; y--) {
        draw_line(x_start, y, x_end, y, color);
        x_start -= inverse_slope_1;
        x_end -= inverse_slope_2;
    }
}

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // Sort the vertices by y-coordinate ascending (y0 < y1 < <2)
    if (y0 > y1) {
        integer_swap(&y0, &y1);
        integer_swap(&x0, &x1);
    }
    if (y1 > y2) {
        integer_swap(&y1, &y2);
        integer_swap(&x1, &x2);
    }
    if (y0 > y1) {
        integer_swap(&y0, &y1);
        integer_swap(&x0, &x1);
    }

    if (y1 == y2) {
        fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    } else if (y0 == y1) {
        fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    } else {
        // Calculate the new vertex (Mx,My) using triangle similarity
        float my = y1;
        float mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

        // Draw flat-bottom triangle
        fill_flat_bottom_triangle(x0, y0, x1, y1, mx, my, color);

        // Draw flat-top triangle
        fill_flat_top_triangle(x1, y1, mx, my, x2, y2, color);
    }    
}
