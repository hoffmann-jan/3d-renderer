#ifndef MESH_H
#define MESH_H
#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

typedef struct {
    // dynamic array of verticies
    vec3_t* vertices;
    // dynamic array of faces
    face_t* faces;
    // rotation with x, y, and z values
    vec3_t rotation;
    // scale with x, y, and z values
    vec3_t scale;
    // translation with x, y, and z values
    vec3_t translation;
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data(void);
void load_obj_file(char* filename);

#endif
