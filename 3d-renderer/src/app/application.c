#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "render-mode.h"
#include "../data-structures/array.h"
#include "../data-structures/mesh.h"
#include "../data-structures/vector.h"

/////////////////////////////////////////////////////////////////////
// Array of triangles that should be rendered frame by frame
/////////////////////////////////////////////////////////////////////
triangle_t* triangles_to_render = NULL;

/////////////////////////////////////////////////////////////////////
// Global variables for execution status and game loop
/////////////////////////////////////////////////////////////////////
bool is_running = false;
int previus_frame_time = 0;

vec3_t camera_position = { .x = 0, .y = 0, .z = 0 };
float fov_factor = 640; 

render_mode_t render_mode;

/////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
/////////////////////////////////////////////////////////////////////
void setup(void) {
	// Allocate the required memory in bytes to hold the color buffer 
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	// Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

    render_mode_initialize(&render_mode);

	// Loads the cube values in the mesh data structure
	// load_cube_mesh_data();
	load_obj_file("./assets/f22.obj");
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)	{
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
            if (event.key.keysym.sym == SDLK_1)
                render_mode_set_wireframe_with_verticies_mode(&render_mode);
            if (event.key.keysym.sym == SDLK_2)
                render_mode_set_wireframe_mode(&render_mode);
            if (event.key.keysym.sym == SDLK_3)
                render_mode_set_filled_triangles_mode(&render_mode);
            if (event.key.keysym.sym == SDLK_4)
                render_mode_set_filled_triangles_with_wireframe_mode(&render_mode);
            if (event.key.keysym.sym == SDLK_c)
                render_mode_enable_back_face_culling(&render_mode);
            if (event.key.keysym.sym == SDLK_d)
                render_mode_disable_back_face_culling(&render_mode);
			break;	
	}
}

////////////////////////////////////////////////////////////
/// Perspective projection with perspective divide
/// P'x = Px / Pz
/// P'y = Py / Pz
////////////////////////////////////////////////////////////
vec2_t project(vec3_t point) {
	vec2_t projected_point = {
		.x = (fov_factor * point.x) / point.z,
		.y = (fov_factor * point.y) / point.z
	};
	return projected_point;
}

void update(void) {

	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previus_frame_time);

	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	previus_frame_time = SDL_GetTicks();

	triangles_to_render = NULL;

	mesh.rotation.x += 0.02;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.00;

	int number_of_faces = array_length(mesh.faces);
	for (int i = 0; i < number_of_faces; i++) {
		face_t mesh_face = mesh.faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec3_t transformend_verticies[3];

		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];

			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			transformed_vertex.z += 5;

            transformend_verticies[j] = transformed_vertex;
        }

        if (render_mode.back_face_culling) {
            // Check backface culling
            vec3_t vector_a = transformend_verticies[0]; /*   A   */
            vec3_t vector_b = transformend_verticies[1]; /*  / \  */
            vec3_t vector_c = transformend_verticies[2]; /* C---B */

            vec3_t vector_ab = vec3_subtract(vector_b, vector_a);
            vec3_t vector_ac = vec3_subtract(vector_c, vector_a);

            vec3_t face_normal = vec3_cross_product(vector_ab, vector_ac);
            vec3_normalize(&face_normal);
            vec3_t camara_ray = vec3_subtract(camera_position, vector_a);

            float dot_product = vec3_dot_product(face_normal, camara_ray);

            if (dot_product < 0) {
                continue;
            }
        }

		triangle_t projected_triangle;

        for (int j = 0; j < 3; j++) {
			vec2_t projected_point = project(transformend_verticies[j]);

			projected_point.x += (window_width / 2),
			projected_point.y += (window_height / 2),

			projected_triangle.points[j] = projected_point;
		}

		array_push(triangles_to_render, projected_triangle);
 	}
}

void render(void) {
	draw_grid(0xFF333333);

	int number_of_triangles = array_length(triangles_to_render);
	for (int i = 0; i < number_of_triangles; i++)
	{
		triangle_t triangle = triangles_to_render[i];

        if (render_mode.filled_triangles_mode) {
            draw_filled_triangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                0xFFA9A9A9
            );
        }
        if (render_mode.filled_triangles_wireframe_mode) {
            draw_filled_triangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                0xFFA9A9A9
            );
            draw_triangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                0xFF00FF00
            );
        }
        if (render_mode.wireframe_mode) {
            draw_triangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                0xFF00FF00
            );
        }
        if (render_mode.wireframe_verticies_mode) {
            draw_triangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                0xFF00FF00
            );
            draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00);
            draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00);
            draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00);
        }
	}

	array_free(triangles_to_render);
	
	render_color_buffer();

	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

// Free the memory that was dynamically allocated by the program
void free_resources(void) {
	free(color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
}

int run(void) {
	is_running = initialize_window();

	setup();

	while(is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();
	free_resources();

	return 0;
}
