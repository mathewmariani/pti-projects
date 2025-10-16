#pragma once

#include <array>
#include <cmath>
#include <memory>

template<typename T>
struct CoordXY {
	T x, y;
	static inline const CoordXY<T> Zero{0, 0};
	static inline const CoordXY<T> Up{+0, +1};
	static inline const CoordXY<T> Right{+1, +0};
	static inline const CoordXY<T> Down{+0, -1};
	static inline const CoordXY<T> Left{-1, +0};

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
		float dx = static_cast<float>(target.x - x);
		float dy = static_cast<float>(target.y - y);
		float len = std::sqrt(dx * dx + dy * dy);
		if (len == 0.0f) return {0.0f, 0.0f};
		return {dx / len, dy / len};
	}
};

template<>
struct std::hash<CoordXY<int>> {
	std::size_t operator()(const CoordXY<int> &location) const noexcept {
		return std::hash<int>()(location.x ^ (location.y << 16));
	}
};