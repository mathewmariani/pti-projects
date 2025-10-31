#pragma once

#include "../actor.h"

struct Goomba : Actor {
	static void Create(const CoordXY<int> &location);

	void Update() override;
	void Render() override;

private:
	void HandleHorizontalMovement();
	void HandleVerticalMovement();
};