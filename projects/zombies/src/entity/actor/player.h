#pragma once

#include "../actor.h"

constexpr float kPlayerMaxSpeed = 0.8f;
constexpr float kPlayerAcceleration = 5.0f;
constexpr float kPlayerFriction = 15.0f;
constexpr float kPlayerFireRate = 0.25f;

constexpr int kPlayerOffsetX = 4;
constexpr int kPlayerOffsetY = 8;
constexpr int kPlayerWidth = 8;
constexpr int kPlayerHeight = 8;
constexpr int kPlayerFrameCount = 8;
constexpr int kPlayerFrameMod = 2;

constexpr float kPlayerShootingKnockback = 1.2f;
constexpr float kPlayerHurtKnockback = 4.0f;

enum class PlayerState : uint8_t {
	Normal,
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

	void Hurt(const CoordXY<float> &direction);

	void Update() override;
	void Render() override;

private:
	void HandleHorizontalMovement();
	void HandleVerticalMovement();

private:
	PlayerState state;
	float shoot_timer;
	int health = 5;
};