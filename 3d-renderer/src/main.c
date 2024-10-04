#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"

/////////////////////////////////////////////////////////////////////
// Array of triangles that should be rendered frame by frame
/////////////////////////////////////////////////////////////////////
triangle_t* triangles_to_render = NULL;

/////////////////////////////////////////////////////////////////////
// Global variables for execution status and game loop
/////////////////////////////////////////////////////////////////////
vec3_t camera_position = { .x = 0, .y = 0, .z = -5 };
float fov_factor = 640;

bool is_running = false;
int previus_frame_time = 0;

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

	// Loads the cube values in the mesh data structure
	load_cube_mesh_data();
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

	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;

	int number_of_faces = array_length(mesh.faces);
	for (int i = 0; i < number_of_faces; i++) {
		face_t mesh_face = mesh.faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		triangle_t projected_triangle;

		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];

			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			transformed_vertex.z += camera_position.z;

			vec2_t projected_point = project(transformed_vertex);

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
		draw_triangle(
			triangle.points[0].x, triangle.points[0].y,
			triangle.points[1].x, triangle.points[1].y,
			triangle.points[2].x, triangle.points[2].y,
			0xFFFFFF00
		);
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

int main(void) {
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
