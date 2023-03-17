#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <memory>
#include <iostream>

#pragma once

#pragma warning(push, 0)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#pragma warning(pop)
#undef main

#include "breezygrass.h"
#include "types.h"
#include "graphics.h"

bool renderRect(const Vector2<int> start, const Vector2<int> size, const RGB& color) {
	return renderRect(start, size, RGBA{ color.R, color.G, color.B, SDL_ALPHA_OPAQUE });
}

bool renderRect(const Vector2<int> start, const Vector2<int> size, const RGBA& color) {
	// Draw a rectangle
	//---

	SDL_Rect rect = SDL_Rect{ start.x(), start.y(), size.x(), size.y()};

	// store current colour
	RGBA current_color{};
	SDL_GetRenderDrawColor(renderer, &current_color.R, &current_color.G, &current_color.B, &current_color.A);

	// set given colour
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);

	// draw
	int ret = SDL_RenderDrawRect(renderer, &rect);

	// restore original colour
	SDL_SetRenderDrawColor(renderer, current_color.R, current_color.G, current_color.B, current_color.A);

	// error handling
	if (ret != 0)
	{
		const char* error = SDL_GetError();
		if (*error != '\0')
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not renderRect. SDL Error: %s at line #%d of file %s/n", error, __LINE__, __FILE__);
			SDL_ClearError();
		}
		return false;
	}

	return true;
}

bool renderFillRect(const Vector2<int> start, const Vector2<int> size, const RGB& color) {
	return renderFillRect(start, size, RGBA{ color.R, color.G, color.B, SDL_ALPHA_OPAQUE });
}

bool renderFillRect(const Vector2<int> start, const Vector2<int> size, const RGBA& color) {
	// Draw a rectangle
	//---

	SDL_Rect rect = SDL_Rect{ start.x(), start.y(), size.x(), size.y()};

	// store current colour
	RGBA current_color{};
	SDL_GetRenderDrawColor(renderer, &current_color.R, &current_color.G, &current_color.B, &current_color.A);

	// set given colour
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);

	// draw
	int ret = SDL_RenderFillRect(renderer, &rect);

	// restore original colour
	SDL_SetRenderDrawColor(renderer, current_color.R, current_color.G, current_color.B, current_color.A);

	// error handling
	if (ret != 0)
	{
		const char* error = SDL_GetError();
		if (*error != '\0')
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not renderRect. SDL Error: %s at line #%d of file %s/n", error, __LINE__, __FILE__);
			SDL_ClearError();
		}
		return false;
	}

	return true;
}

bool renderLine(const Vector2<int> start, const Vector2<int> end, const RGB& color) {
	RGB oldc;

	int ww, wh;
	SDL_GetWindowSize(window,
		&ww,
		&wh);

	// Draw a line
	//---
	int ret = SDL_RenderDrawLine(
		renderer, // SDL_Renderer* renderer: the renderer in which draw
		start.x(),               // int x1: x of the starting point
		start.y(),          // int y1: y of the starting point
		end.x(),                 // int x2: x of the end point
		end.y());           // int y2: y of the end point

	// error handling
	if (ret != 0)
	{
		const char* error = SDL_GetError();
		if (*error != '\0')
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not renderDrawLine. SDL Error: %s at line #%d of file %s/n", error, __LINE__, __FILE__);
			SDL_ClearError();
		}
		return false;
	}

	return true;
}

bool renderLine(const Vector2<float> start, const Vector2<float> end, const RGB& color) {
	RGB oldc;

	int ww, wh;
	SDL_GetWindowSize(window,
		&ww,
		&wh);

	// Draw a line
	//---
	int ret = SDL_RenderDrawLine(
		renderer, // SDL_Renderer* renderer: the renderer in which draw
		static_cast<int>(round(start.x())),               // int x1: x of the starting point
		static_cast<int>(round(start.y())),          // int y1: y of the starting point
		static_cast<int>(round(end.x())),                 // int x2: x of the end point
		static_cast<int>(round(end.y())));           // int y2: y of the end point

	// error handling
	if (ret != 0)
	{
		const char* error = SDL_GetError();
		if (*error != '\0')
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not renderDrawLine. SDL Error: %s at line #%d of file %s/n", error, __LINE__, __FILE__);
			SDL_ClearError();
		}
		return false;
	}

	return true;
}

void renderCircle(const Vector2<int> center, float radius, const RGB& color, unsigned int sides) {
	if (sides == 0)
	{
		sides = static_cast<unsigned int>(round(TWOPI * radius / 2));
	}

	float d_a = TWOPI / sides;
	float angle = d_a;

	Vector2<int32_t> start{ 0, 0 };
	Vector2<int32_t> end{ 0 , 0 };
	end.set_x(static_cast<int32_t>(radius));
	end.set_y(0);
	end = end + center;

	for (int i = 0; i != sides; i++)
	{
		start = end;
		end.set_x(static_cast<int32_t>(cos(angle) * radius));
		end.set_y(static_cast<int32_t>(sin(angle) * radius));
		end = end + center;
		angle += d_a;
		renderLine(start, end, color);
	}
}