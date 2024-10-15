#include "matrix.h"

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

vec4_t matrix_4by4_multiply_with_vec4(matrix_4by4_t m, vec4_t v) {
    vec4_t result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;

    return result;
}
