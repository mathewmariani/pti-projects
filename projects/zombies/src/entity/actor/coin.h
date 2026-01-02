#pragma once

#include "batteries/actor.h"

struct Coin : Actor {
	static void Create(const CoordXY<int> &location);

	Coin();

	void Collect();

	void Update() override;
	void Render() override;
};