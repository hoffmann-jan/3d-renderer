#ifndef SETTINGS_H
#define SETTINGS_H

enum render_mode_e {
    WIREFRAME,
    WIREFRAME_VERTICIES,
    FILLED_TRIANGLES,
    FILLED_TRIANGLES_WIREFRAME
};

enum culling_e {
    NO_CULLING,
    BACK_FACE_CULLING
};

#endif
