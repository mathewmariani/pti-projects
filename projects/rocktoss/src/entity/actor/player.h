#pragma once

#include "batteries/actor.h"
#include "rock.h"

struct Player : Actor {
	enum class State : uint8_t {
		Normal,
		Jump,
		Death,
	};

	Player();

	void Hurt(const CoordXY<float> &direction);
	void Kill();

	void Update() override;
	void Render() override;

private:
	void HandleHorizontalMovement();
	void HandleVerticalMovement();
	void HandleJump();
	void HandlePickup();

private:
	State state;

	Rock *held;

	// celeste:
	float jumpBuffer = 0.0f;
	float coyoteTime = 0.0f;
};