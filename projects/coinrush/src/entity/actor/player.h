#pragma once

#include "batteries/actor.h"

struct Player : Actor {
	enum class State : uint8_t {
		Normal,
		Jump,
		Death,
	};

	Player();

	void Hurt(const CoordXY<float> &direction);

	void Update() override;
	void Render() override;

private:
	void HandleHorizontalMovement();
	void HandleVerticalMovement();
	void HandleJump();

private:
	State state;

	// celeste:
	float jumpBuffer = 0.0f;
	float coyoteTime = 0.0f;
};