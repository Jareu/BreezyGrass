#pragma once

#include "types.h"

void renderCircle(const Vector2<int> center, float radius, const RGB& color, unsigned int sides);
bool renderLine(const Vector2<float> start, const Vector2<float> end, const RGB& color);
bool renderLine(const Vector2<int> start, const Vector2<int> end, const RGB& color);
bool renderRect(const Vector2<int> start, const Vector2<int> end, const RGB& color);
bool renderRect(const Vector2<int> start, const Vector2<int> end, const RGBA& color);
bool renderFillRect(const Vector2<int> start, const Vector2<int> end, const RGB& color);
bool renderFillRect(const Vector2<int> start, const Vector2<int> end, const RGBA& color);