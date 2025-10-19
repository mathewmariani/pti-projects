#pragma once

#include "pti/pti.h"

#include "coordinate.h"

#include <map>
#include <vector>

std::vector<CoordXY<int>> VonNewmanNeighborhood(const CoordXY<int> &location) {
	auto neighbors = CoordXY<int>::VonNewmanNeighborhood();
	if ((location.x + location.y) % 2 == 0) {
		std::reverse(neighbors.begin(), neighbors.end());
	}

	std::vector<CoordXY<int>> ret;
	CoordXY<int> next{0, 0};
	for (const auto &delta : neighbors) {
		next = location + delta;
		ret.push_back(next);
	}

	return ret;
}