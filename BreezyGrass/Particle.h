#pragma once
#include "types.h"

class Particle
{
private:
	static constexpr float DEFAULT_LENGTH = 10.f;
public:
	bool locked = false;
	float length{ DEFAULT_LENGTH };
	Vector2<float> position{ 0.f, 0.f };
	Vector2<float> velocity{ 0.f, 0.f };
	Vector2<float> spring_force{ 0.f, 0.f };
	float angle_radians{ 0.f };
	float mass_kg{ 0.5f };
	float speed{ 0.f };

	Particle() = default;
	void update(double elapsed_time);
};