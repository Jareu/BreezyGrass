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

	SDL_ShowWindow(window);

	while (is_running) {
		handleEvents();
		update();
		if (render() == RENDER_RESULT::RENDER_FAILED) {
			is_running = false;
			break;
		}
	}


	// frees memory associated with renderer and window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	// destroy textures
	SDL_DestroyTexture(sim_texture);

	IMG_Quit();
	SDL_Quit();

	return 0;
}

void update() {
	// do physics
}


// Render the Game
int render() {
	if (!is_active) RENDER_RESULT::RENDER_SUCCESS;

	SDL_SetRenderDrawColor(renderer, 26, 26, 32, 255);
	SDL_RenderClear(renderer);

	// draw border
	SDL_SetRenderTarget(renderer, sim_texture);

	if (!sim_texture) {
		sim_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window_size.x, window_size.y);
		if (!sim_texture) {
			std::cout << "Error creating sim texture: " << SDL_GetError() << "\n";
			return RENDER_RESULT::RENDER_FAILED;
		}
	}

	// draw to the texture
	SDL_SetRenderTarget(renderer, sim_texture);

	// fill surface with black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, NULL);

	// renderCircle(screen_coords, star_radius_large, star->GetColour(), 4);

	// renderRect(Vector2{ 0, 0 }, ceiling_size, RGBA{ 128, 128, 200, 128 });

	// draw segments
	/*
	for (int y = 0; y < ceiling_size.y; y += segment_size) {
		renderLine(Vector2{ 0, y }, Vector2{ ceiling_size.x, y }, RGB{ 100, 100, 160 });
	}*/

	SDL_RenderCopy(renderer, sim_texture, NULL, &sim_rect);
	SDL_RenderPresent(renderer);

	return RENDER_RESULT::RENDER_SUCCESS;
}



// handles any events that SDL noticed.
void handleEvents() {
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