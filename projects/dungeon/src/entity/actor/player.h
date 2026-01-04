#pragma once

#include "batteries/actor.h"

enum class PlayerState : uint8_t {
	Normal,
	Death,
};

struct Player : Actor {
	Player();

	void Hurt(const CoordXY<float> &direction);

	void Update() override;
	void Render() override;

private:
	void HandleHorizontalMovement();
	void HandleVerticalMovement();

private:
	PlayerState state;
};