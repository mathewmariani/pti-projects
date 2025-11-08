#pragma once

#include <cstdlib>  // for std::rand, std::srand
#include <ctime>    // for std::time
#include <algorithm>// for std::swap

inline void InitRandom() {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

inline int RandomRange(int a, int b) {
	if (a > b) std::swap(a, b);
	return a + std::rand() % (b - a + 1);
}