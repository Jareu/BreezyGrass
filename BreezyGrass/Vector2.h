#pragma once

#include "defines.h"

template <typename T>
class Vector2 {
public:
	explicit Vector2() {
		Vector2(static_cast<T>(0.f), static_cast<T>(0.f));
	}

	Vector2(T x, T y) : x_{ x }, y_{ y } {
		static_assert(std::is_arithmetic<T>::value, "Vector2 must be numeric type.");
	};

	float magnitude() {
		// if magnitude is out of date, recalculate it
		if (magnitude_stale_) {
			magnitude_ = sqrtf(static_cast<float>(x_ * x_ + y_ * y_));
			magnitude_stale_ = false;
		}

		return magnitude_;
	}

	void normalize() {
		magnitude(); // ensure magnitude is up to date
		if (abs(magnitude_) < INFINITESIMAL) {
			return;
		}
		x_ = x_ / magnitude_;
		y_ = y_ / magnitude_;
		magnitude_ = 1.f;
	}

	Vector2<T> rotated_90() {
		return Vector2{ -y_, x_ };
	}

	const T& x() const {
		return x_;
	}

	const T& y() const {
		return y_;
	}

	void set_x(const T& val) {
		magnitude_stale_ = true;
		x_ = val;
	}

	void set_y(const T& val) {
		magnitude_stale_ = true;
		y_ = val;
	}

	// cross product with vector2
	template <typename B>
	const T operator^(const Vector2<B>& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator +");
		const T cross_product = this->x_ * static_cast<T> (rhs.y_) - this->y_ * static_cast<T> (rhs.x_);
		return cross_product;
	}

	// addition with vector2
	template <typename B>
	const Vector2<T> operator+(const Vector2<B>& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator +");
		return Vector2<T>{
			this->x_ + static_cast<T> (rhs.x_),
				this->y_ + static_cast<T> (rhs.y_)
		};
	}

	// addition and assignment with vector2
	template <typename B>
	Vector2<T>& operator+=(const Vector2<B>& rhs) {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator -");
		this->x_ = this->x_ + static_cast<T> (rhs.x_);
		this->y_ = this->y_ + static_cast<T> (rhs.y_);
		return *this;
	}

	// subtraction with vector2
	template <typename B>
	const Vector2<T> operator-(const Vector2<B>& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator -");
		const auto subtraction = Vector2<T>{
			this->x_ - static_cast<T> (rhs.x_),
			this->y_ - static_cast<T> (rhs.y_)
		};
		return subtraction;
	}

	// subtraction and assignment with vector2
	template <typename B>
	Vector2<T>& operator-=(const Vector2<B>& rhs) {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator -");
		this->x_ = this->x_ - static_cast<T> (rhs.x_);
		this->y_ = this->y_ - static_cast<T> (rhs.y_);
		return *this;
	}

	// multiplication with scalar
	template <typename B>
	const Vector2<T> operator*(const B& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator *");
		return Vector2<T>{
			this->x_* static_cast<T> (rhs),
				this->y_* static_cast<T> (rhs)
		};
	}

	// multiplication with vector2
	const Vector2<T> operator*(const Vector2<T>& rhs) const {
		return Vector2<T>{
			this->x_* static_cast<T> (rhs.x_),
				this->y_* static_cast<T> (rhs.y_)
		};
	}

	// multiplication and assignment with scalar
	template <typename B>
	Vector2& operator*=(const B& rhs) {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator *=");
		this->x_ = this->x_ * static_cast<T> (rhs);
		this->y_ = this->y_ * static_cast<T> (rhs);
		return *this;
	}

	// division with scalar
	template <typename B>
	const Vector2 operator/(const B& rhs) const {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator /");
		return Vector2{
			this->x_ / static_cast<T> (rhs),
			this->y_ / static_cast<T> (rhs)
		};
	}

	// division with vector2
	const Vector2<T> operator/(const Vector2<T>& rhs) const {
		return Vector2<T>{
			this->x_ / static_cast<T> (rhs.x_),
				this->y_ / static_cast<T> (rhs.y_)
		};
	}


	// division and assignment with scalar
	template <typename B>
	Vector2& operator/=(const B& rhs) {
		static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator /=");
		this->x_ = this->x_ / static_cast<T> (rhs);
		this->y_ = this->y_ / static_cast<T> (rhs);
		return *this;
	}
private:
	T x_ = 0;
	T y_ = 0;
	bool magnitude_stale_ = true;
	float magnitude_ = 0.f;
};

// multiplication with scalar from LHS
template <typename T, typename B>
inline const Vector2<T> operator*(const B& lhs, const Vector2<T>& rhs) {
	static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator *");
	return Vector2<T>{
		rhs.x()* static_cast<T> (lhs),
			rhs.y()* static_cast<T> (lhs)
	};
}


// division with scalar from LHS
template <typename T, typename B>
inline const Vector2<T> operator/(const B& lhs, const Vector2<T>& rhs) {
	static_assert(std::is_arithmetic<B>::value, "RHS must be numeric type for operator /");
	return Vector2<T>{
		rhs.x() / static_cast<T> (lhs),
			rhs.y() / static_cast<T> (lhs)
	};
}