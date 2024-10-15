#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "settings.h"
#include "../algorithms/painters-algorithm.h"
#include "../data-structures/array.h"
#include "../data-structures/matrix.h"
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

enum render_mode_e render_mode;
enum culling_e culling_mode;

/////////////////////////////////////////////////////////////////////
// Setup function to initialize variables and game objects
/////////////////////////////////////////////////////////////////////
void setup(void) {
	// Allocate the required memory in bytes to hold the color buffer 
	color_buffer = (color_t*)malloc(sizeof(color_t) * window_width * window_height);

	// Creating a SDL texture that is used to display the color buffer
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

    render_mode = WIREFRAME;
    culling_mode = BACK_FACE_CULLING;

	// Loads the cube values in the mesh data structure
	load_cube_mesh_data();
	// load_obj_file("./assets/f22.obj");
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
                render_mode = WIREFRAME_VERTICIES;
            if (event.key.keysym.sym == SDLK_2)
                render_mode = WIREFRAME;
            if (event.key.keysym.sym == SDLK_3)
                render_mode = FILLED_TRIANGLES;
            if (event.key.keysym.sym == SDLK_4)
                render_mode = FILLED_TRIANGLES_WIREFRAME;
            if (event.key.keysym.sym == SDLK_c)
                culling_mode = BACK_FACE_CULLING;
            if (event.key.keysym.sym == SDLK_d)
                culling_mode = NO_CULLING;
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

    // change the mesh scale/rotation per animation frame
	mesh.rotation.x += 0.02;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.00;
    mesh.scale.x += 0.002;
    mesh.scale.y += 0.001;

    // Create a scale matrix that will be used to multiply the mesh vertices
    matrix_4by4_t scale_matrix = matrix_4by4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z, 1);

	int number_of_faces = array_length(mesh.faces);
	for (int i = 0; i < number_of_faces; i++) {
		face_t mesh_face = mesh.faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec4_t transformend_verticies[3];

		for (int j = 0; j < 3; j++) {
			vec4_t transformed_vertex = vec4_from_vec3(&face_vertices[j]);

            // use matrix to scale original vertex
            transformed_vertex = matrix_4by4_multiply_with_vec4(scale_matrix, transformed_vertex);

            // translate the vertex away from camera
			transformed_vertex.z += 5;

            transformend_verticies[j] = transformed_vertex;
        }

        if (render_mode == BACK_FACE_CULLING) {
            // Check backface culling
            vec3_t vector_a = vec3_from_vec4(&transformend_verticies[0]); /*   A   */
            vec3_t vector_b = vec3_from_vec4(&transformend_verticies[1]); /*  / \  */
            vec3_t vector_c = vec3_from_vec4(&transformend_verticies[2]); /* C---B */

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

        vec2_t projected_points[3];

        for (int j = 0; j < 3; j++) {
			projected_points[j] = project(vec3_from_vec4(&transformend_verticies[j]));

			projected_points[j].x += (window_width / 2);
			projected_points[j].y += (window_height / 2);
        }

        float avg_depth = (transformend_verticies[0].z + transformend_verticies[1].z + transformend_verticies[2].z) / 3;

        triangle_t projected_triangle = {
            .points = {
                projected_points[0],
                projected_points[1],
                projected_points[2]   
            },
            .color = mesh_face.color,
            .average_depth = avg_depth
        };
        array_push(triangles_to_render, projected_triangle);
 	}
    painters_algorithm(triangles_to_render);
}

void render(void) {
	draw_grid(0xFF333333);

	int number_of_triangles = array_length(triangles_to_render);
	for (int i = 0; i < number_of_triangles; i++)
	{
		triangle_t triangle = triangles_to_render[i];

        if (render_mode == FILLED_TRIANGLES) {
            draw_filled_triangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                triangle.color
            );
        }
        if (render_mode == FILLED_TRIANGLES_WIREFRAME) {
            draw_filled_triangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                triangle.color
            );
            draw_triangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                0xFF00FF00
            );
        }
        if (render_mode == WIREFRAME) {
            draw_triangle(
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                0xFF00FF00
            );
        }
        if (render_mode == WIREFRAME_VERTICIES) {
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
