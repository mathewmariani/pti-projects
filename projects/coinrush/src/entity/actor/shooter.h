#pragma once

#include "../actor.h"

struct Shooter : Actor {
	Shooter();

	void Update() override;
	void Render() override;

private:
	void HandleHorizontalMovement();
	void HandleVerticalMovement();

private:
	float shoot_timer;
};