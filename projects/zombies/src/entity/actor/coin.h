#pragma once

#include "../actor.h"

struct Coin : Actor {
	static void Create(const CoordXY<int> &location);

	Coin();

	void Update() override;
	void Render() override;
};