#pragma once

#include "../actor.h"

constexpr float kZombieMaxSpeed = 3.0f;
constexpr float kZombieAcceleration = 20.0f;
constexpr float kZombieFriction = 15.0f;
constexpr float kZombiePhysicsVerticalMax = 6.0f;
constexpr float kZombiePhysicsVerticalGrav = 0.24f;
constexpr float kZombiePhysicsVerticalGravFall = 0.4254f;
constexpr float kZombiePhysicsVerticalGravAlt = 0.1940f;
constexpr float kZombiePhysicsJumpStrength = 6.1135f;
constexpr float kZombiePhysicsBounceStrength = 4.5535f;

constexpr float kZombieJumpBuffer = 0.1334f;
constexpr float kZombieCoyoteTime = 0.1;

constexpr int kZombieOffsetX = 4;
constexpr int kZombieOffsetY = 8;
constexpr int kZombieWidth = 8;
constexpr int kZombieHeight = 8;
constexpr int kZombieFrameCount = 8;
constexpr int kZombieFrameMod = 2;

enum class ZombieState : uint8_t {
	Normal,
};

struct Zombie : Actor {
	Zombie() {
		bx = 0;
		by = 0;
		bw = kZombieWidth;
		bh = kZombieHeight;
		state = ZombieState::Normal;
	}

	void Update() override;
	void Render() override;

private:
	ZombieState state;
};