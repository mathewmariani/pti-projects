#include "rock.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

#include "batteries/juice.h"

constexpr float kRockMaxSpeed = 3.0f;
constexpr float kRockAcceleration = 20.0f;
constexpr float kRockPhysicsVerticalMax = 6.0f;
constexpr float kRockPhysicsVerticalGrav = 0.24f;
constexpr float kRockPhysicsVerticalGravFall = 0.5f;
constexpr float kRockPhysicsVerticalGravAlt = 0.1940f;
constexpr float kRockPhysicsJumpStrength = 4.7397f;
constexpr float kRockPhysicsBounceStrength = 4.5535f;

constexpr float kRockJumpBuffer = 0.1334f;
constexpr float kRockCoyoteTime = 0.1;

constexpr int kRockHitboxOffsetX = -4;
constexpr int kRockHitboxOffsetY = -4;
constexpr int kRockHitboxWidth = 8;
constexpr int kRockHitboxHeight = 8;

constexpr int kRockOffsetX = 4;
constexpr int kRockOffsetY = 4;
constexpr int kRockWidth = 8;
constexpr int kRockHeight = 8;

constexpr int kRockFrameCount = 8;
constexpr int kRockFrameMod = 2;

constexpr float kRockShootingKnockback = 0.85f;
constexpr float kRockHurtKnockback = 4.0f;

constexpr float kRockFriction = 5.0f;
constexpr float kRockThrowSpeed = 4.7397f * 2.0f;// initial speed multiplier
constexpr float kRockGravity = 10.0f;            // units per frame²
constexpr float kRockVerticalMax = 15.0f;        // terminal velocity

Rock::Rock() {
	bx = kRockHitboxOffsetX;
	by = kRockHitboxOffsetY;
	bw = kRockHitboxWidth;
	bh = kRockHitboxHeight;
}

void Rock::Update() {
	if (owner) {
		position = owner->position;
		return;
	}
	HandleHorizontalMovement();
	HandleVerticalMovement();
}

void Rock::Render() {
	pti_spr(bitmap_bullet, 0, position.x - kRockOffsetX, position.y - kRockOffsetY, false, false);
}

void Rock::Pickup(const Actor *actor) {
	if (owner) {
		return;
	}

	owner = actor;
	grounded = false;
	speed = CoordXY<float>::Zero;
}

void Rock::Throw(const CoordXY<float> &dir) {
	speed = dir * kRockThrowSpeed;
	owner = nullptr;
	grounded = false;
}

void Rock::HaltX() {
	speed.x = -speed.x * 0.75f;
}
void Rock::HaltY() {
	speed.y = 0.0f;
}

void Rock::HandleHorizontalMovement() {
	_pti_appr(speed.x, 0, kRockFriction * PTI_DELTA);
}

void Rock::HandleVerticalMovement() {
	if (!grounded) {
		speed.y += kRockGravity * PTI_DELTA;
		if (speed.y > kRockVerticalMax) {
			speed.y = kRockVerticalMax;
		}
	} else {
		speed.y = 0.0f;
	}
}