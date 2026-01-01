#pragma once

#include "batteries/solid.h"

struct Shooter : Solid {
	Shooter(const CoordXY<int> &dir);

	void Update() override;
	void Render() override;

private:
	void HandleHorizontalMovement();
	void HandleVerticalMovement();

private:
	float shoot_timer;
	CoordXY<int> shoot_direction;
};