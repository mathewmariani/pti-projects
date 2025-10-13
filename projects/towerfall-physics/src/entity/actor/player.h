#pragma once

#include "../actor.h"

constexpr float kPlayerMaxSpeed = 3.0f;
constexpr float kPlayerAcceleration = 20.0f;
constexpr float kPlayerFriction = 15.0f;
constexpr float kPlayerPhysicsVerticalMax = 6.0f;
constexpr float kPlayerPhysicsVerticalGrav = 0.24f;
constexpr float kPlayerPhysicsVerticalGravFall = 0.4254f;
constexpr float kPlayerPhysicsVerticalGravAlt = 0.1940f;
constexpr float kPlayerPhysicsJumpStrength = 6.1135f;
constexpr float kPlayerPhysicsBounceStrength = 4.5535f;

constexpr float kPlayerJumpBuffer = 0.1334f;
constexpr float kPlayerCoyoteTime = 0.1;

constexpr int kPlayerOffsetX = 4;
constexpr int kPlayerOffsetY = 8;
constexpr int kPlayerWidth = 8;
constexpr int kPlayerHeight = 8;
constexpr int kPlayerFrameCount = 8;
constexpr int kPlayerFrameMod = 2;

enum class PlayerState : uint8_t {
	Normal,
	Jump,
	Death,
};

struct Player : Actor {
	Player() {
		bx = 0;
		by = 0;
		bw = kPlayerWidth;
		bh = kPlayerHeight;
		state = PlayerState::Normal;
	}

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