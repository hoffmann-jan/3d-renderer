#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct {
    float m[4][4];
} matrix_4by4_t;

matrix_4by4_t matrix_4by4_make_identity(void);
matrix_4by4_t matrix_4by4_make_scale(float scale_x, float scale_y, float scale_z, float scale_w);
matrix_4by4_t matrix_4by4_make_translation(float translate_x, float translate_y, float translate_z);
matrix_4by4_t matrix_4by4_make_rotation_x(float angle);
matrix_4by4_t matrix_4by4_make_rotation_y(float angle);
matrix_4by4_t matrix_4by4_make_rotation_z(float angle);

matrix_4by4_t matrix_4by4_multiply_by_matrix_4by4(matrix_4by4_t m1, matrix_4by4_t m2);
vec4_t matrix_4by4_multiply_by_vec4(matrix_4by4_t m, vec4_t v);

#endif
