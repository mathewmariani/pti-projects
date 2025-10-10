#pragma once

#include "../solid.h"

constexpr float kPlatformRange = 56.0f;
constexpr float kPlatformMaxSpeed = 12.0f;
constexpr float kPlatformAcceleration = 20.0f;
constexpr float kPlatformFriction = 15.0f;
constexpr float kPlatformPhysicsVerticalMax = 6.0f;
constexpr float kPlatformPhysicsVerticalGrav = 0.24f;
constexpr float kPlatformPhysicsVerticalGravFall = 0.4254f;
constexpr float kPlatformPhysicsVerticalGravAlt = 0.1940f;
constexpr float kPlatformPhysicsJumpStrength = 6.1135f;
constexpr float kPlatformPhysicsBounceStrength = 4.5535f;

constexpr int kPlatformOffsetX = 4;
constexpr int kPlatformOffsetY = 8;
constexpr int kPlatformWidth = 8;
constexpr int kPlatformHeight = 8;
constexpr int kPlatformFrameCount = 8;
constexpr int kPlatformFrameMod = 2;


struct Platform : Solid {
	static constexpr auto cEntityType = EntityType::Platform;

	enum class Type {
		Horizontal,
		Vertical
	};

	Platform(const Type t) {
		bx = 0;
		by = 0;
		bw = kPlatformWidth;
		bh = kPlatformHeight;
		direction = (t == Type::Horizontal) ? CoordXY{+1, 0} : CoordXY{0, -1};
		type = t;
	}

	void Update() override;
	void Render() override;

	Type type;
};