#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <typeinfo>
#include <stdint.h>
#include <math.h>
#include <cstdint>

#include "Vector2.h"

enum RENDER_RESULT {
	RENDER_SUCCESS = 0,
	RENDER_FAILED = 1
};

constexpr uint16_t SPRING_K = 2000;
constexpr uint16_t SPRING_D = 20;



template <typename T>
class Vector3 {
public:
	T x = 0;
	T y = 0;
	T z = 0;

	Vector3() = delete;
	Vector3(T x, T y, T z) : x{ x }, y{ y }, z{ z } {
		static_assert(std::is_arithmetic<T>::value, "Vector3 must be numeric type.");
	};


	// addition with same type
	const Vector3 operator+(const Vector3& rhs) const {
		return Vector3{
			this->x + rhs.x,
			this->y + rhs.y,
			this->z + rhs.z
		};
	}

	// addition with different type
	template <typename B>
	const Vector3 operator+(const Vector3<B>& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator +");
		return Vector3{
			this->x + static_cast<T> (rhs.x),
			this->y + static_cast<T> (rhs.y),
			this->z + static_cast<T> (rhs.z)
		};
	}

	// addition and assignment with same type
	Vector3& operator+=(const Vector3& rhs) {
		this->x = this->x + rhs.x;
		this->y = this->y + rhs.y;
		this->z = this->z + rhs.z;
		return *this;
	}

	// subtraction with same type
	const Vector3 operator-(const Vector3& rhs) const {
		return Vector3{
			this->x - rhs.x,
			this->y - rhs.y,
			this->z - rhs.z
		};
	}

	// subtraction with different type
	template <typename B>
	const Vector3 operator-(const Vector3<B>& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator -");
		return Vector3{
			this->x - static_cast<T> (rhs.x),
			this->y - static_cast<T> (rhs.y),
			this->z - static_cast<T> (rhs.z)
		};
	}

	// subtraction and assignment with same type
	Vector3& operator-=(const Vector3& rhs) {
		this->x = this->x - rhs.x;
		this->y = this->y - rhs.y;
		this->z = this->z - rhs.z;
		return *this;
	}

	// multiplication with scalar
	template <typename B>
	Vector3 operator*(const B& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator *");
		return Vector3{
			this->x * static_cast<T> (rhs),
			this->y * static_cast<T> (rhs),
			this->z * static_cast<T> (rhs)
		};
	}

	// multiplication and assignment with scalar
	template <typename B>
	Vector3& operator*=(const B& rhs) {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator *=");
		this->x = this->x * static_cast<T> (rhs);
		this->y = this->y * static_cast<T> (rhs);
		this->z = this->z * static_cast<T> (rhs);
		return *this;
	}

	// division with scalar
	template <typename B>
	Vector3 operator/(const B& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator /");
		return Vector3{
			this->x / static_cast<T> (rhs),
			this->y / static_cast<T> (rhs),
			this->z / static_cast<T> (rhs)
		};
	}

	// division and assignment with scalar
	template <typename B>
	Vector3& operator/=(const B& rhs) {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator /=");
		this->x = this->x / static_cast<T> (rhs);
		this->y = this->y / static_cast<T> (rhs);
		this->z = this->z / static_cast<T> (rhs);
		return *this;
	}
};

class VectorSpherical {
public:
	float theta = 0.f;
	float phi = 0.f;

	VectorSpherical() = default;

	VectorSpherical(float theta, float phi) : theta{ theta }, phi{ phi } {};

	VectorSpherical operator+(const VectorSpherical& other) {
		VectorSpherical sum = VectorSpherical(this->theta + other.theta, this->phi + other.phi);
		return sum;
	}

	VectorSpherical operator-(const VectorSpherical& other) {
		VectorSpherical sum = VectorSpherical(this->theta - other.theta, this->phi - other.phi);
		return sum;
	}
};

template <typename T>
class Range {
public:
	T min = 0;
	T max = 0;

	Range() = default;
	Range(T min, T max) : min{ min }, max{ max } {};
};

struct RGB {
	uint8_t R = 0;
	uint8_t G = 0;
	uint8_t B = 0;
};

struct RGBA {
	uint8_t R = 0;
	uint8_t G = 0;
	uint8_t B = 0;
	uint8_t A = 0;
};

struct HSL {
	float H = 0;
	float S = 0;
	float L = 0;
};

// Fonts
enum class eFontSize {
	SMALL,
	MEDIUM,
	LARGE,
	TITLE
};

typedef struct _EndPointRef {
	int	ref;
	Vector2<float> pt;
} EndPoint;

typedef struct _Spring
{
	EndPoint end1;
	EndPoint end2;
	float k = SPRING_K;
	float damping = SPRING_D;
	float nominal_length{0.f};
} Spring;