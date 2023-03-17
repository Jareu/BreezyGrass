#pragma once

#include <string>
#include <utility>
#include <vector>
#include <chrono>

#include "Particle.h"
#include "types.h"

#define TWOPI 6.2831853071f

constexpr uint8_t NUM_OBJECTS = 50;
constexpr uint8_t NUM_SPRINGS = NUM_OBJECTS-1;

inline SDL_Renderer* renderer = nullptr;
inline SDL_Window* window = nullptr;
inline int WINDOW_WIDTH = 1920;
inline int WINDOW_HEIGHT = 1080;
inline Vector2<int> window_size = { WINDOW_WIDTH, WINDOW_HEIGHT };
inline SDL_Texture* sim_texture = nullptr;
inline bool is_active = false;
inline bool is_running = false;
inline bool is_fullscreen = true;
inline SDL_Rect sim_rect = SDL_Rect{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
inline std::chrono::steady_clock::time_point last_frame_time {};
inline Particle objects[NUM_OBJECTS];
inline Spring springs[NUM_SPRINGS];
inline float scale = 100.f; // pixels per meter

int		main();
void	handleEvents();
void	update(double dt);
float	get_elapsed_time();
int		render();
bool	initialize_objects();