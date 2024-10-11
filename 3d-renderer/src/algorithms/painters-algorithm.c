#include "painters-algorithm.h"
#include "bubble-sort.h"
#include "quick-sort.h"
#include "../data-structures/array.h"

void painters_algorithm(triangle_t triangles[]) {
    bubble_sort(triangles);
    // quick_sort(triangles, 0, array_length(triangles) - 1);
}
