#include "quick-sort.h"


void swap(triangle_t *a, triangle_t *b) {
    triangle_t tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(triangle_t triangles[], int low, int high) {
    triangle_t pivot = triangles[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (triangles[j].average_depth >= pivot.average_depth) {
            i++;
            swap(&triangles[i], &triangles[j]);
        }
    }
    swap(&triangles[i + 1], &triangles[high]);
    return (i + 1);
}

void quick_sort(triangle_t triangles[], int low, int high) {
    if (low < high) {
        int partition_index = partition(triangles, low, high);

        quick_sort(triangles, low, partition_index - 1);
        quick_sort(triangles, partition_index + 1, high);
    }
}
