#pragma once

#include "../actor.h"

constexpr float kPlayerMaxSpeed = 3.0f;
constexpr float kPlayerAcceleration = 20.0f;
constexpr float kPlayerFriction = 15.0f;

constexpr int kPlayerOffsetX = 4;
constexpr int kPlayerOffsetY = 8;
constexpr int kPlayerWidth = 8;
constexpr int kPlayerHeight = 8;
constexpr int kPlayerFrameCount = 8;
constexpr int kPlayerFrameMod = 2;

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

	void Update() override;
	void Render() override;

private:
	void HandleHorizontalMovement();
	void HandleVerticalMovement();

private:
	PlayerState state;
};