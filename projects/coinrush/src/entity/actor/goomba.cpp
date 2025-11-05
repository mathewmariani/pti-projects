#include "pti/pti.h"

#include "goomba.h"
#include "effect.h"
#include "../../gamestate.h"
#include "../../bank.h"

constexpr float kGoombaMaxSpeed = 1.0f;
constexpr float kGoombaAcceleration = 20.0f;
constexpr float kGoombaFriction = 15.0f;
constexpr float kGoombaPhysicsVerticalMax = 6.0f;
constexpr float kGoombaPhysicsVerticalGrav = 0.24f;
constexpr float kGoombaPhysicsVerticalGravFall = 0.4254f;
constexpr float kGoombaPhysicsVerticalGravAlt = 0.1940f;
constexpr float kGoombaPhysicsJumpStrength = 6.1135f;
constexpr float kGoombaPhysicsBounceStrength = 4.5535f;

constexpr int kGoombaHitboxOffsetX = -4;
constexpr int kGoombaHitboxOffsetY = -4;
constexpr int kGoombaHitboxWidth = 8;
constexpr int kGoombaHitboxHeight = 8;

constexpr int kGoombaOffsetX = 6;
constexpr int kGoombaOffsetY = 12;
constexpr int kGoombaFrameCount = 8;
constexpr int kGoombaFrameMod = 2;

void Goomba::Create(const CoordXY<int> &location) {
	auto *goomba = CreateEntity<Goomba>();
	if (goomba == nullptr) {
		return;
	}

	goomba->SetLocation(location);
}

Goomba::Goomba() {
	bx = kGoombaHitboxOffsetX;
	by = kGoombaHitboxOffsetY;
	bw = kGoombaHitboxWidth;
	bh = kGoombaHitboxHeight;
	direction.x = -1;
}

void Goomba::Update() {
	HandleHorizontalMovement();
	HandleVerticalMovement();

	const CoordXY<int> horizontal{direction.x, 0};
	if (PlaceMeeting(horizontal) || CollidesWithSolids(horizontal)) {
		direction.x *= -1;
		speed.x = -speed.x;
	}

	for (auto *player : GetCollisions<Player>(this, direction)) {
		player->Hurt(CoordXY<float>::Zero);
	}
}

void Goomba::Render() {
	auto frame = static_cast<int>(timer * kGoombaFrameCount) % kGoombaFrameMod;
	if (speed.x == 0 && IsGrounded()) {
		frame = 0;
	}

	auto flip_x = direction.x < 0;
	pti_spr(bitmap_goomba, frame, position.x - kGoombaOffsetX, position.y - kGoombaOffsetY, flip_x, false);
}

void Goomba::HandleHorizontalMovement() {
	_pti_appr(speed.x, direction.x * kGoombaMaxSpeed, kGoombaAcceleration * PTI_DELTA);
}

void Goomba::HandleVerticalMovement() {
	speed.y += kGoombaPhysicsVerticalGravFall;
	if (speed.y > kGoombaPhysicsVerticalMax) {
		speed.y = kGoombaPhysicsVerticalMax;
	}
}