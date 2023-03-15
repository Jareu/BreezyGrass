#pragma once

#include <string>
#include <utility>
#include <vector>

#include "types.h"

#define TWOPI 6.2831853071f
inline SDL_Renderer* renderer = NULL;
inline SDL_Window* window = NULL;
inline int WINDOW_WIDTH = 1920;
inline int WINDOW_HEIGHT = 1080;
inline Vector2<int> window_size = { WINDOW_WIDTH, WINDOW_HEIGHT };
inline SDL_Texture* sim_texture = NULL;
inline bool is_active = false;
inline bool is_running = false;
inline bool is_fullscreen = false;
inline SDL_Rect sim_rect = SDL_Rect{ 0,0,0,0 };

enum RENDER_RESULT {
	RENDER_SUCCESS = 0,
	RENDER_FAILED = 1
};

int main();
void handleEvents();
void update();
int render();
