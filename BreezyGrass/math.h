#pragma once
#include <math.h>
#include "types.h"

template <typename T>
Vector2<T> rotate_2D(const double angle_radians, const Vector2<T>& point)
{
	const double sina = sin(angle_radians);
	const double cosa = cos(angle_radians);

	const Vector2<double> rotated_point{
		point.x() * cosa - point.y() * sina,
		point.x() * sina + point.y() * cosa
	};

	return static_cast<Vector2<T>(rotated_point);
}