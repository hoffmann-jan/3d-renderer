#include "bubble-sort.h"

#include "../data-structures/array.h"

void bubble_sort(triangle_t triangles[]) {
    int number_of_triangles = array_length(triangles);
    for (int i = 0; i < number_of_triangles; i++) {
        for (int j = 0; j < number_of_triangles; j++) {
            if (triangles[i].average_depth > triangles[j].average_depth) {
                triangle_t temp = triangles[i];
                triangles[i] = triangles[j];
                triangles[j] = temp;
            }
        }
    }
}
