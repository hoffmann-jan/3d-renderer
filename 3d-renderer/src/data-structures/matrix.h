#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct {
    float m[4][4];
} matrix_4by4_t;

matrix_4by4_t matrix_4by4_make_identity(void);
matrix_4by4_t matrix_4by4_make_scale(float scale_x, float scale_y, float scale_z, float scale_w);

vec4_t matrix_4by4_multiply_with_vec4(matrix_4by4_t m, vec4_t v);

#endif
