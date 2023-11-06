#include "RigidBody2D.h"
#include "breezygrass.h"

void RigidBody2D::update(double elapsed_time)
{
	Vector2<float> a, dv, ds;

	// Integrate equation of motion:
	// motion and position of point 1
	// rotation
	// position of point 2

	if (!locked) {
		a = spring_force / mass_kg + Vector2<float>{ 0.f, scale * 9.81f };
		dv = a * elapsed_time;
		velocity += dv;
		ds = velocity * elapsed_time;
		position += ds;
		// Misc. calculations:
		speed = velocity.magnitude();
	}
}