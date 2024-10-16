#include "matrix.h"

#include<math.h>

// | 1 0 0 0 |
// | 0 1 0 0 |
// | 0 0 1 0 |
// | 0 0 0 1 |
matrix_4by4_t matrix_4by4_make_identity(void) {
    matrix_4by4_t identity = {{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    }};
    return identity;
}

// | sx 0 0 0 |
// | 0 sy 0 0 |
// | 0 0 sz 0 |
// | 0 0 0 sw |
matrix_4by4_t matrix_4by4_make_scale(float scale_x, float scale_y, float scale_z, float scale_w) {
    matrix_4by4_t scale = matrix_4by4_make_identity();
    scale.m[0][0] = scale_x;
    scale.m[1][1] = scale_y;
    scale.m[2][2] = scale_z;
    scale.m[3][3] = scale_w;
    return scale;
}

// | 1 0 0 tx |
// | 0 1 0 ty |
// | 0 0 1 tz |
// | 0 0 0  1 |
matrix_4by4_t matrix_4by4_make_translation(float translate_x, float translate_y, float translate_z) {
    matrix_4by4_t translation = matrix_4by4_make_identity();
    translation.m[0][3] = translate_x;
    translation.m[1][3] = translate_y;
    translation.m[2][3] = translate_z;
    return translation;
}

// |  1  0  0  0 |
// |  0  c -s  0 |
// |  0  s  c  0 |
// |  0  0  0  1 |
matrix_4by4_t matrix_4by4_make_rotation_x(float angle) {
    float cosine = cos(angle);
    float sine = sin(angle);
    matrix_4by4_t rotation_x = matrix_4by4_make_identity();
    rotation_x.m[1][1] = cosine;
    rotation_x.m[1][2] = -sine;
    rotation_x.m[2][1] = sine;
    rotation_x.m[2][2] = cosine;
    return rotation_x;
}

// |  c  0  s  0 |
// |  0  1  0  0 |
// | -s  0  c  0 |
// |  0  0  0  1 |
matrix_4by4_t matrix_4by4_make_rotation_y(float angle) {
    float cosine = cos(angle);
    float sine = sin(angle);
    matrix_4by4_t rotation_y = matrix_4by4_make_identity();
    rotation_y.m[0][0] = cosine;
    rotation_y.m[0][2] = sine;
    rotation_y.m[2][0] = -sine;
    rotation_y.m[2][2] = cosine;
    return rotation_y;
}

// |  c -s  0  0 |
// |  s  c  0  0 |
// |  0  0  1  0 |
// |  0  0  0  1 |
matrix_4by4_t matrix_4by4_make_rotation_z(float angle) {
    float cosine = cos(angle);
    float sine = sin(angle);
    matrix_4by4_t rotation_z = matrix_4by4_make_identity();
    rotation_z.m[0][0] = cosine;
    rotation_z.m[0][1] = -sine;
    rotation_z.m[1][0] = sine;
    rotation_z.m[1][1] = cosine;
    return rotation_z;
}

matrix_4by4_t matrix_4by4_multiply_by_matrix_4by4(matrix_4by4_t a, matrix_4by4_t b) {
    matrix_4by4_t result_matrix;
    for (int row = 0; row < 4; row++) {
        for (int column = 0; column < 4; column++) {
            result_matrix.m[row][column] = a.m[row][0] * b.m[0][column]
                                         + a.m[row][1] * b.m[1][column]
                                         + a.m[row][2] * b.m[2][column]
                                         + a.m[row][3] * b.m[3][column];
        }
    }
    return result_matrix;
}

vec4_t matrix_4by4_multiply_by_vec4(matrix_4by4_t m, vec4_t v) {
    vec4_t result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;

    return result;
}
