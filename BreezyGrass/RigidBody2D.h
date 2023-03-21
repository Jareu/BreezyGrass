#pragma once
#include "types.h"

class RigidBody2D
{
private:
	static constexpr float DEFAULT_LENGTH = 10.f;
public:
	bool locked = false;
	float length{ DEFAULT_LENGTH };
	Vector2<float> position{ 0.f, 0.f };
	Vector2<float> velocity{ 0.f, 0.f };
	Vector2<float> angular_velocity{ 0.f, 0.f };
	Vector2<float> spring_force{ 0.f, 0.f };
	Vector2<float> spring_moment{ 0.f, 0.f };
	float angle{ 0.f }; // radians
	float mass_kg{ 0.5f };
	float speed{ 0.f };

	RigidBody2D() = default;
	void update(double elapsed_time);
};