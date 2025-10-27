#pragma once

#include <algorithm>
#include <random>

int RandomRange(int a, int b) {
	static thread_local std::mt19937 gen(std::random_device{}());
	if (a > b) {
		std::swap(a, b);
	}

	std::uniform_int_distribution<int> dist(a, b);
	return dist(gen);
}