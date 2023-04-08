// BreezyGrass.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma warning(push, 0)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#pragma warning(pop)
#undef main

#include <string>
#include <stdlib.h>     /* srand, rand */
#include <iostream>

#include "math.h"
#include "types.h"
#include "graphics.h"
#include "breezygrass.h"

int main()
{
	int SDL_RENDERER_FLAGS = 0;
	int SDL_WINDOW_INDEX = -1;

	int SDL_WINDOW_FLAGS = 0;
	SDL_WINDOW_FLAGS = SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;

	if (is_fullscreen) {
		SDL_WINDOW_FLAGS = SDL_WINDOW_FLAGS | SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		// SDL failed. Output error message and exit
		std::cout << "Failed to initialize SDL:" << SDL_GetError() << "\n";
		return EXIT_FAILURE;
	}

	// Create Window
	window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FLAGS);
	if (!window) {
		std::cout << "Failed to create window: " << SDL_GetError() << "\n";
		return EXIT_FAILURE;
	}

	// set window size
	SDL_SetWindowMinimumSize(window, 100, 100);
	SDL_GetWindowSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);

	// Create Renderer
	renderer = SDL_CreateRenderer(window, SDL_WINDOW_INDEX, SDL_RENDERER_FLAGS);
	if (!renderer) {
		std::cout << "Failed to create renderer: " << SDL_GetError() << "\n";
		return EXIT_FAILURE;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderClear(renderer); // initialize backbuffer
	is_running = true; // everything was set up successfully

	initialize_objects();

	SDL_ShowWindow(window);
	SDL_RenderFillRect(renderer, NULL);

	while (is_running) {
		handleEvents();
		float elapsed_time = get_elapsed_time();
		update(elapsed_time);
		if (render() == RENDER_RESULT::RENDER_FAILED) {
			is_running = false;
			break;
		}
	}

	// frees memory associated with renderer and window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;

	// destroy textures
	if(sim_texture) {
		SDL_DestroyTexture(sim_texture);
		sim_texture = nullptr;
	}

	IMG_Quit();
	SDL_Quit();

	return 0;
}

float get_elapsed_time()
{
	float elapsed_time;
	auto elapsed = std::chrono::high_resolution_clock::now() - last_frame_time;
	int64_t microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	last_frame_time = std::chrono::high_resolution_clock::now();
	elapsed_time = static_cast<float>(microseconds / 1000000.0);
	return elapsed_time;
}

// Do physics
void update(double dt)
{
	int j;

	// Initialize the spring forces on each object to zero.
	for (int i = 0; i < NUM_LINKS; i++) {
		objects[i].spring_force = { 0.f, 0.f };
		objects[i].spring_moment = 0.f;
	}

	// Calculate all spring forces based on positions of connected objects.
	for (int i = 0; i < NUM_SPRINGS; i++)
	{
		double f, dl, M;
		Vector2<float> Fo{}, pt1{}, pt2{}, r{}, F{}, v1{}, v2{}, vr{};

		if (springs[i].end1.ref < 0) {
			pt1 = springs[i].end1.pt;
		}
		else {
			j = springs[i].end1.ref;
			pt1 = objects[j].position + rotate_2D(objects[j].angle, springs[i].end1.pt);
			v1 = objects[j].velocity + rotate_2D(objects[j].angle, objects[j].angular_velocity * springs[i].end1.pt).rotated_90();
		}

		if (springs[i].end2.ref < 0) {
			pt2 = springs[i].end2.pt;
		}
		else {
			j = springs[i].end2.ref;
			pt2 = objects[j].position + rotate_2D(objects[j].angle, springs[i].end2.pt);
			v2 = objects[j].velocity + rotate_2D(objects[j].angle, objects[j].angular_velocity * springs[i].end2.pt).rotated_90();
		}

		// Compute spring-damper-force
		vr = v2 - v1;
		r = pt2 - pt1;

		if (abs(r.magnitude()) < INFINITESIMAL) {
			// don't calculate for point springs
			continue;
		}

		dl = r.magnitude() - springs[i].nominal_length;
		f = springs[i].k * dl; // - means compression, + means tension
		r.normalize();
		F = (r * f) + (springs[i].damping * (vr * r)) * r;

		j = springs[i].end1.ref;
		if (j >= 0) {
			objects[j].spring_force += F;
		}

		j = springs[i].end2.ref;
		if (j >= 0) {
			objects[j].spring_force -= F;
		}

		// convert force to first ref local coords
		// Get local lever
		// calc moment
		// 
		// Compute and aggregate moments due to spring force
		// on each connected object.
		j = springs[i].end1.ref;
		if (j >= 0) {
			Fo = rotate_2D(-objects[j].angle, F);
			r = springs[i].end1.pt;
			M = r ^ Fo;
			objects[j].spring_moment += static_cast<float>(M);
		}

		j = springs[i].end2.ref;
		if (j >= 0) {
			Fo = rotate_2D(-objects[j].angle, F);
			r = springs[i].end2.pt;
			M = r ^ Fo;
			objects[j].spring_moment -= static_cast<float>(M);
		}

	}

	for (int i = 0; i < NUM_LINKS; i++) {
		objects[i].update(dt);
	}
}

// Render the Game
int render()
{
	if (!is_active) RENDER_RESULT::RENDER_SUCCESS;

	SDL_SetRenderDrawColor(renderer, 26, 26, 32, 255);
	SDL_RenderClear(renderer);

	// fill surface with black
	SDL_SetRenderDrawColor(renderer, 200, 0, 0, SDL_ALPHA_OPAQUE);

	// draw rope
	for (int i = 0; i < NUM_SPRINGS; i++) {
		renderLine( objects[i].position, objects[i+1].position, RGB{200, 0, 0});
	}
	
	SDL_RenderPresent(renderer);

	return RENDER_RESULT::RENDER_SUCCESS;
}

bool initialize_objects()
{
	Vector2<float> nominal_length {};
	Vector2<float> pt {};
	int i;

	// set frame time
	last_frame_time = std::chrono::high_resolution_clock::now();

	// Initialize particle locations from left to right.
	for (i = 0; i < NUM_LINKS; i++)
	{
		objects[i] = RigidBody2D{};
		objects[i].position.set_x( WINDOW_WIDTH / 2.f + objects[0].length * i );
		objects[i].position.set_y( WINDOW_HEIGHT / 8.f );
		objects[i].angle = 0.f;
	}

	// Connect end of the first object to a fixed point in space.
	springs[0].end1.ref = -1;
	springs[0].end1.pt.set_x( (WINDOW_WIDTH - objects[0].length) * 0.5f );
	springs[0].end1.pt.set_y( WINDOW_HEIGHT / 8.f );

	springs[0].end2.ref = 0;
	springs[0].end2.pt.set_x (-objects[0].length * 0.5f);
	springs[0].end2.pt.set_y (0.f);

	pt = rotate_2D(objects[0].angle, springs[0].end2.pt) + objects[0].position;
	nominal_length = pt - springs[0].end1.pt;
	springs[0].nominal_length = nominal_length.magnitude();
	
	// Initialize springs connecting particles from left to right.
	for (i = 1; i < NUM_SPRINGS; i++)
	{
		springs[i].end1.ref = i - 1;
		springs[i].end1.pt.set_x(objects[i-1].length * 0.5f);
		springs[i].end1.pt.set_y(0.f);

		springs[i].end2.ref = i;
		springs[i].end2.pt.set_x(-objects[i].length * 0.5f);
		springs[i].end2.pt.set_y(0.f);

		pt = rotate_2D(objects[i].angle, springs[i].end2.pt) + objects[i].position;
		nominal_length = pt - rotate_2D(objects[i - 1].angle, springs[i].end1.pt) + objects[i - 1].position;
		springs[i].nominal_length = nominal_length.magnitude();
	}

	return true;
}


// handles any events that SDL noticed.
void handleEvents()
{
	//the only event we'll check is the  SDL_QUIT event.
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type) {
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				is_running = false;
			}
			break;
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_WINDOWEVENT:
			switch (event.window.event) {
			case SDL_WINDOWEVENT_FOCUS_LOST:

				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_RESTORED:
			case SDL_WINDOWEVENT_SHOWN:
				//case SDL_WINDOWEVENT_EXPOSED:
				is_active = true;
				break;
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
				is_active = false;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}