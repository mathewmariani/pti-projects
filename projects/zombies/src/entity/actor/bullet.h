#pragma once

#include "../actor.h"

constexpr float kBulletMaxSpeed = 3.0f;
constexpr float kBulletAcceleration = 20.0f;
constexpr float kBulletFriction = 15.0f;
constexpr float kBulletPhysicsVerticalMax = 6.0f;
constexpr float kBulletPhysicsVerticalGrav = 0.24f;
constexpr float kBulletPhysicsVerticalGravFall = 0.4254f;
constexpr float kBulletPhysicsVerticalGravAlt = 0.1940f;
constexpr float kBulletPhysicsJumpStrength = 6.1135f;
constexpr float kBulletPhysicsBounceStrength = 4.5535f;

constexpr float kBulletJumpBuffer = 0.1334f;
constexpr float kBulletCoyoteTime = 0.1;

constexpr int kBulletOffsetX = 4;
constexpr int kBulletOffsetY = 8;
constexpr int kBulletWidth = 8;
constexpr int kBulletHeight = 8;
constexpr int kBulletFrameCount = 8;
constexpr int kBulletFrameMod = 2;

struct Bullet : Actor {
	Bullet() {
		bx = 0;
		by = 0;
		bw = kBulletWidth;
		bh = kBulletHeight;
	}

	void Update() override;
	void Render() override;
};