// BreezyGrass.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma warning(push, 0)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#pragma warning(pop)
#undef main

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
	int i, j;
	double f, dl;
	Vector2<float> pt1{}, pt2{}, r{}, F{}, v1{}, v2{}, vr{};

	// Initialize the spring forces on each object to zero.
	for (i = 0; i < NUM_OBJECTS; i++) {
		objects[i].spring_force.set_x(0.f);
		objects[i].spring_force.set_y(0.f);
	}

	// Calculate all spring forces based on positions of connected objects.
	for (i = 0; i < NUM_SPRINGS; i++)
	{
		j = springs[i].end1;
		pt1 = objects[j].position;
		v1 = objects[j].velocity;

		j = springs[i].end2;
		pt2 = objects[j].position;
		v2 = objects[j].velocity;

		vr = v2 - v1;
		r = pt2 - pt1;
		dl = r.magnitude() - springs[i].nominal_length;
		f = springs[i].k * dl; // - means compression, + means tension
		r.normalize();
		F = (r * f) + (springs[i].damping * (vr * r)) * r;

		j = springs[i].end1;
		objects[j].spring_force += F;

		j = springs[i].end2;
		objects[j].spring_force -= F;
	}

	for (i = 0; i < NUM_OBJECTS; i++) {
		objects[i].update(dt);
	}
}

// Render the Game
int render()
{
	if (!is_active) RENDER_RESULT::RENDER_SUCCESS;

	SDL_SetRenderDrawColor(renderer, 26, 26, 32, 255);
	SDL_RenderClear(renderer);

	/*
	if (!sim_texture) {
		sim_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window_size.x(), window_size.y());
		if (!sim_texture) {
			std::cout << "Error creating sim texture: " << SDL_GetError() << "\n";
			return RENDER_RESULT::RENDER_FAILED;
		}
	}
	*/

	// draw to the texture
	//SDL_SetRenderTarget(renderer, sim_texture);

	// fill surface with black
	SDL_SetRenderDrawColor(renderer, 200, 0, 0, SDL_ALPHA_OPAQUE);

	// draw rope
	for (int i = 0; i < NUM_SPRINGS; i++) {
		renderLine( objects[i].position, objects[i+1].position, RGB{200, 0, 0});
	}

	//SDL_RenderCopy(renderer, sim_texture, NULL, &sim_rect);
	
	SDL_RenderPresent(renderer);

	return RENDER_RESULT::RENDER_SUCCESS;
}

bool initialize_objects()
{
	Vector2<float> nominal_length {};
	int i;

	// set frame time
	last_frame_time = std::chrono::high_resolution_clock::now();

	// Initialize particle locations from left to right.
	for (i = 0; i < NUM_OBJECTS; i++)
	{
		objects[i] = Particle{};
		objects[i].position.set_x( WINDOW_WIDTH / 2.f + objects[0].length * i );
		objects[i].position.set_y( WINDOW_HEIGHT / 8.f );
	}

	objects[0].locked = true;

	// Initialize springs connecting particles from left to right.
	for (i = 0; i < NUM_SPRINGS; i++)
	{
		springs[i].end1 = i;
		springs[i].end2 = i + 1;
		nominal_length = objects[i + 1].position - objects[i].position;
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