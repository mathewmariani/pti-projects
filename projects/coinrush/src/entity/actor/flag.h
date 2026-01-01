#pragma once

#include "batteries/actor.h"

struct Flag : Actor {
	static void Create(const CoordXY<int> &location);

	Flag();

	void Update() override;
	void Render() override;

	bool CanBeMoved(void) const override { return false; }
};