#pragma once

#include "../actor.h"

constexpr float kBulletMaxSpeed = 1.0f;
constexpr float kBulletAcceleration = 20.0f;
constexpr int kBulletBoundaryOffset = 2;

constexpr int kBulletOffsetX = 2;
constexpr int kBulletOffsetY = 2;
constexpr int kBulletWidth = 4;
constexpr int kBulletHeight = 4;
constexpr int kBulletFrameCount = 1;
constexpr int kBulletFrameMod = 1;

struct Bullet : Actor {
	Bullet() {
		bx = 0;
		by = 0;
		bw = kBulletWidth;
		bh = kBulletHeight;
		direction = CoordXY<int>::Right;
	}

	void Update() override;
	void Render() override;
};