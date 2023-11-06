#pragma once

#include <utility>
#include <vector>
#include <chrono>

#include "SDL.h"
#include "RigidBody2D.h"
#include "types.h"
#include "defines.h"

constexpr uint8_t NUM_LINKS = 10;
constexpr uint8_t NUM_SPRINGS = NUM_LINKS;

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
inline RigidBody2D objects[NUM_LINKS];
inline Spring springs[NUM_SPRINGS];
inline float scale = 100.f; // pixels per meter

int		main();
void	handleEvents();
void	update(double dt);
float	get_elapsed_time();
int		render();
bool	initialize_objects();