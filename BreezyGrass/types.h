#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <typeinfo>
#include <stdint.h>
#include <math.h>
#include <cstdint>

enum RENDER_RESULT {
	RENDER_SUCCESS = 0,
	RENDER_FAILED = 1
};

constexpr uint16_t SPRING_K = 1000;
constexpr uint16_t SPRING_D = 100;

namespace PARTICLE_DEFAULTS {
	constexpr float length = 100;
}

template <typename T>
class Vector2 {
public:
	T x = 0;
	T y = 0;

	Vector2() {
		Vector2(static_cast<T>(0.f), static_cast<T>(0.f));
	}

	Vector2(T x, T y) : x{ x }, y{ y } {
		static_assert(std::is_arithmetic<T>::value, "Vector2 must be numeric type.");
	};

	// addition with same type
	const Vector2 operator+(const Vector2& rhs) const {
		return Vector2{
			this->x + rhs.x,
			this->y + rhs.y
		};
	}

	// addition with different type
	template <typename B>
	const Vector2 operator+(const Vector2<B>& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator +");
		return Vector2{
			this->x + static_cast<T> (rhs.x),
			this->y + static_cast<T> (rhs.y)
		};
	}

	// addition and assignment with same type
	Vector2& operator+=(const Vector2& rhs) {
		this->x = this->x + rhs.x;
		this->y = this->y + rhs.y;
		return *this;
	}

	// subtraction with same type
	const Vector2 operator-(const Vector2& rhs) const {
		return Vector2{
			this->x - rhs.x,
			this->y - rhs.y
		};
	}

	// subtraction with different type
	template <typename B>
	const Vector2 operator-(const Vector2<B>& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator -");
		return Vector2{
			this->x - static_cast<T> (rhs.x),
			this->y - static_cast<T> (rhs.y)
		};
	}

	// subtraction and assignment with same type
	Vector2& operator-=(const Vector2& rhs) {
		this->x = this->x - rhs.x;
		this->y = this->y - rhs.y;
		return *this;
	}

	// multiplication with scalar
	template <typename B>
	Vector2 operator*(const B& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator *");
		return Vector2{
			this->x * static_cast<T> (rhs),
			this->y * static_cast<T> (rhs)
		};
	}

	// multiplication and assignment with scalar
	template <typename B>
	Vector2& operator*=(const B& rhs) {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator *=");
		this->x = this->x * static_cast<T> (rhs);
		this->y = this->y * static_cast<T> (rhs);
		return *this;
	}

	// division with scalar
	template <typename B>
	Vector2 operator/(const B& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator /");
		return Vector2{
			this->x / static_cast<T> (rhs),
			this->y / static_cast<T> (rhs)
		};
	}

	// division and assignment with scalar
	template <typename B>
	Vector2& operator/=(const B& rhs) {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator /=");
		this->x = this->x / static_cast<T> (rhs);
		this->y = this->y / static_cast<T> (rhs);
		return *this;
	}
};

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

typedef struct _Spring
{
	int end1{0};
	int end2{0};
	float k = SPRING_K;
	float damping = SPRING_D;
	float nominal_length{0.f};
} Spring;

typedef struct _Particle
{
	bool locked = false;
	float length{ 0.f };
	Vector2<float> position{ 0.f, 0.f };
	Vector2<float> velocity{ 0.f, 0.f };
	Vector2<float> spring_force{ 0.f, 0.f };
	float angle_radians{0.f};
	float mass_kg{ 1.f };
} Particle;