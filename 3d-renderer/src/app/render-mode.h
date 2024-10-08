#ifndef RENDER_MODE_H
#define RENDER_MODE_H

#include <stdbool.h>

typedef struct {
    bool wireframe_mode;
    bool wireframe_verticies_mode;
    bool filled_triangles_mode;
    bool filled_triangles_wireframe_mode;
    bool back_face_culling;
} render_mode_t;

void render_mode_initialize(render_mode_t *mode);

void render_mode_set_wireframe_with_verticies_mode(render_mode_t *mode);
void render_mode_set_wireframe_mode(render_mode_t *mode);
void render_mode_set_filled_triangles_mode(render_mode_t *mode);
void render_mode_set_filled_triangles_with_wireframe_mode(render_mode_t *mode);

void render_mode_enable_back_face_culling(render_mode_t *mode);
void render_mode_disable_back_face_culling(render_mode_t *mode);

#endif
