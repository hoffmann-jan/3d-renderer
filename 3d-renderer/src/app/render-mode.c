#include "render-mode.h"

void render_mode_initialize(render_mode_t *mode) {
    mode->back_face_culling = false;
    mode->filled_triangles_mode = true;
    mode->filled_triangles_wireframe_mode = false;
    mode->wireframe_mode = false;
    mode->wireframe_verticies_mode = false;
}

void render_mode_set_wireframe_with_verticies_mode(render_mode_t *mode) {
    mode->filled_triangles_mode = false;
    mode->filled_triangles_wireframe_mode = false;
    mode->wireframe_mode = false;
    mode->wireframe_verticies_mode = true;
}

void render_mode_set_wireframe_mode(render_mode_t *mode) {
    mode->filled_triangles_mode = false;
    mode->filled_triangles_wireframe_mode = false;
    mode->wireframe_mode = true;
    mode->wireframe_verticies_mode = false;
}

void render_mode_set_filled_triangles_mode(render_mode_t *mode) {
    mode->filled_triangles_mode = true;
    mode->filled_triangles_wireframe_mode = false;
    mode->wireframe_mode = false;
    mode->wireframe_verticies_mode = false;
}

void render_mode_set_filled_triangles_with_wireframe_mode(render_mode_t *mode) {
    mode->filled_triangles_mode = false;
    mode->filled_triangles_wireframe_mode = true;
    mode->wireframe_mode = false;
    mode->wireframe_verticies_mode = false;    
}

void render_mode_enable_back_face_culling(render_mode_t *mode) {
    mode->back_face_culling = true;
}

void render_mode_disable_back_face_culling(render_mode_t *mode) {
    mode->back_face_culling = false;
}
