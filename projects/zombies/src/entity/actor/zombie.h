#pragma once

#include "../actor.h"

constexpr float kZombieMaxSpeed = 3.0f;
constexpr float kZombieAcceleration = 20.0f;
constexpr float kZombieFriction = 15.0f;
constexpr float kZombieKnockback = 2.0f;

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

	void Hurt(const CoordXY<int> &direction);

	void Update() override;
	void Render() override;

private:
	ZombieState state;
};