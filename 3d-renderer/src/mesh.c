#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = { .x = 0, .y = 0, .z = 0 }
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{ .x = -1, .y = -1, .z = -1 }, // 1
	{ .x = -1, .y =  1, .z = -1 }, // 2
	{ .x =  1, .y =  1, .z = -1 }, // 3
	{ .x =  1, .y = -1, .z = -1 }, // 4
	{ .x =  1, .y =  1, .z =  1 }, // 5
	{ .x =  1, .y = -1, .z =  1 }, // 6
	{ .x = -1, .y =  1, .z =  1 }, // 7
	{ .x = -1, .y = -1, .z =  1 }  // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3 },
    { .a = 1, .b = 3, .c = 4 },
    // right
    { .a = 4, .b = 3, .c = 5 },
    { .a = 4, .b = 5, .c = 6 },
    // back
    { .a = 6, .b = 5, .c = 7 },
    { .a = 6, .b = 7, .c = 8 },
    // left
    { .a = 8, .b = 7, .c = 2 },
    { .a = 8, .b = 2, .c = 1 },
    // top
    { .a = 2, .b = 7, .c = 5 },
    { .a = 2, .b = 5, .c = 3 },
    // bottom
    { .a = 6, .b = 8, .c = 1 },
    { .a = 6, .b = 1, .c = 4 }
};

void load_cube_mesh_data(void) {
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        vec3_t cube_vertex = cube_vertices[i];
        array_push(mesh.vertices, cube_vertex);
    }
    for (int i = 0; i < N_CUBE_FACES; i++) {
        face_t cube_face = cube_faces[i];
        array_push(mesh.faces, cube_face);
    }
}

void load_obj_file(char* filename) {
    FILE *file = NULL;
    char *line = NULL;
    size_t line_length = 0;
    ssize_t read;

    file = fopen(filename, "r");

    if (file != NULL) {
        while ((read = getline(&line, &line_length, file)) != -1) {
            if (strncmp(line, "v ", 2) == 0) {
                vec3_t vertex;
                sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                array_push(mesh.vertices, vertex);
            }

            if (strncmp(line, "f ", 2) == 0) {
                int vertex_indicies[3];
                int texture_indicies[3];
                int normal_indicies[3];
                sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                &vertex_indicies[0], &texture_indicies[0], &normal_indicies[0],
                &vertex_indicies[1], &texture_indicies[1], &normal_indicies[1],
                &vertex_indicies[2], &texture_indicies[2], &normal_indicies[2]);

                face_t face = { .a = vertex_indicies[0], .b = vertex_indicies[1], .c = vertex_indicies[2] };
                array_push(mesh.faces, face);
            }
        }

        free(line);
        fclose(file);
    }
}
