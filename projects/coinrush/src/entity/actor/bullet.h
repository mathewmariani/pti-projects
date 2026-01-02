#pragma once

#include "batteries/actor.h"

struct Bullet : Actor {
	static void Create(const CoordXY<int> &location);

	Bullet();

	void HaltX(void) override;
	void HaltY(void) override;

	void Update() override;
	void Render() override;

	bool CanBeMoved(void) const override { return false; }
};