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

constexpr int kGoombaOffsetX = 4;
constexpr int kGoombaOffsetY = 8;
constexpr int kGoombaWidth = 8;
constexpr int kGoombaHeight = 8;
constexpr int kGoombaFrameCount = 8;
constexpr int kGoombaFrameMod = 2;

void Goomba::Create(const CoordXY<int> &location) {
	auto *goomba = CreateEntity<Goomba>();
	if (goomba == nullptr) {
		return;
	}

	goomba->SetLocation(location);
}


void Goomba::Update() {
	// if (IsTouchingWall()) {
	// 	direction *= -1;
	// 	position.x += direction;
	// 	speed.x = -speed.x;
	// }

	HandleHorizontalMovement();
	HandleVerticalMovement();
}

void Goomba::Render() {
	auto frame = static_cast<int>(timer * kGoombaFrameCount) % kGoombaFrameMod;
	if (speed.x == 0 && IsGrounded()) {
		frame = 0;
	}

	pti_spr(bitmap_goomba, frame, position.x - kGoombaOffsetX, position.y - kGoombaOffsetY, false, false);
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