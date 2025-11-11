#pragma once

#include "../actor.h"

enum class PlayerState : uint8_t {
	Normal,
	Jump,
	Death,
};

struct Player : Actor {
	Player();

	void Update() override;
	void Render() override;

private:
	void HandleHorizontalMovement();
	void HandleVerticalMovement();
	void HandleJump();

private:
	PlayerState state;

	// celeste:
	float jumpBuffer = 0.0f;
	float coyoteTime = 0.0f;
};