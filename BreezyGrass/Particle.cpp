#include "Particle.h"

void Particle::update(double elapsed_time)
{
	Vector2<float> a, dv, ds;

	// Integrate equation of motion:
	if (!locked) {
		a = (spring_force + Vector2<float>{ 0.f, 100.f * 9.81f }) / mass_kg;
		dv = a * elapsed_time;
		velocity += dv;
		ds = velocity * elapsed_time;
		position += ds;
		// Misc. calculations:
		speed = velocity.magnitude();
	}
}