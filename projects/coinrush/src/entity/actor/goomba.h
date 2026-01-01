#pragma once

#include "batteries/actor.h"

struct Goomba : Actor {
	static void Create(const CoordXY<int> &location);

	Goomba();

	void Update() override;
	void Render() override;

private:
	void HandleHorizontalMovement();
	void HandleVerticalMovement();
};