#pragma once

#include "../actor.h"

struct Bullet : Actor {
	static void Create(const CoordXY<int> &location);

	Bullet();

	void Update() override;
	void Render() override;
};