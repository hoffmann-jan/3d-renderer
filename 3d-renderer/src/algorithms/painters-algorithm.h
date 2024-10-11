#ifndef PAINTERS_ALGORITHM_H
#define PAINTERS_ALGORITHM_H

#include "../data-structures/triangle.h"

/// @brief Sorting triangles by average depth to decide rendering seqeunce.
/// @param triangles Array of triangles.
void painters_algorithm(triangle_t triangles[]);

#endif
