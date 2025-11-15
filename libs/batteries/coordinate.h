#pragma once

#include <array>
#include <cmath>
#include <memory>

template<typename T>
struct CoordXY {
	T x, y;
	static inline const CoordXY<T> Zero{0, 0};
	static inline const CoordXY<T> One{+1, +1};
	static inline const CoordXY<T> Up{+0, +1};
	static inline const CoordXY<T> Right{+1, +0};
	static inline const CoordXY<T> Down{+0, -1};
	static inline const CoordXY<T> Left{-1, +0};
	static inline const CoordXY<T> X{1, 0};
	static inline const CoordXY<T> Y{0, 1};

	static constexpr std::array<CoordXY<T>, 4> VonNewmanNeighborhood() {
		return {Up, Left, Down, Right};
	}

	CoordXY() : x(0), y(0) {}
	CoordXY(T x_, T y_) : x(x_), y(y_) {}
	CoordXY(const CoordXY &) = default;
	CoordXY(CoordXY &&) = default;

	CoordXY<T> operator*(const T rhs) const {
		return {x * rhs, y * rhs};
	}
	CoordXY<T> operator+(const CoordXY<T> &rhs) const {
		return {x + rhs.x, y + rhs.y};
	}
	CoordXY<T> operator-(const CoordXY<T> &rhs) const {
		return {x - rhs.x, y - rhs.y};
	}
	CoordXY<T> &operator=(const CoordXY<T> &rhs) {
		x = rhs.x;
		y = rhs.y;
		return (*this);
	};
	bool operator<(const CoordXY &other) const {
		return x < other.x || (x == other.x && y < other.y);
	}
	bool operator==(const CoordXY &other) const {
		return (x == other.x && y == other.y);
	}

	CoordXY<float> DirectionTo(const CoordXY<T> &target) const {
		auto d = target - *this;
		float len = std::sqrt(d.x * d.x + d.y * d.y);
		if (len == 0.0f) return CoordXY<float>::Zero;
		return (d / len);
	}

	T SqrMagnitude() const noexcept {
		return ((x * x) + (y * y));
	}

	float SqrDistance(const CoordXY<T> &target) const noexcept {
		return SqrMagnitude(target - *this);
	}

	static float SqrMagnitude(const CoordXY<T> &v) noexcept {
		return static_cast<T>((v.x * v.x) + (v.y * v.y));
	}
};

template<typename T, typename U>
auto operator*(const CoordXY<T> &lhs, U rhs)
		-> CoordXY<decltype(lhs.x * rhs)> {
	return {lhs.x * rhs, lhs.y * rhs};
}

template<typename T, typename U>
auto operator/(const CoordXY<T> &lhs, U rhs)
		-> CoordXY<decltype(lhs.x * rhs)> {
	return {lhs.x / rhs, lhs.y / rhs};
}

template<typename T, typename U>
auto operator*(U lhs, const CoordXY<T> &rhs)
		-> CoordXY<decltype(lhs * rhs.x)> {
	return {lhs * rhs.x, lhs * rhs.y};
}

template<typename T>
auto operator*(const CoordXY<float> &lhs, const CoordXY<T> &rhs)
		-> CoordXY<float> {
	return {lhs.x * static_cast<float>(rhs.x), lhs.y * static_cast<float>(rhs.y)};
}

template<>
struct std::hash<CoordXY<int>> {
	std::size_t operator()(const CoordXY<int> &location) const noexcept {
		return std::hash<int>()(location.x ^ (location.y << 16));
	}
};