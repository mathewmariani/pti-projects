#include "pti/pti.h"

#include "player.h"

#include "../../bank.h"      /* bitmap_player */
#include "../../gamestate.h" /* PTI_DELTA */

constexpr float kPlayerMaxSpeed = 0.8f;
constexpr float kPlayerAcceleration = 5.0f;
constexpr float kPlayerFriction = 15.0f;
constexpr float kPlayerFireRate = 0.20f;

constexpr int kPlayerHitboxOffsetX = -1;
constexpr int kPlayerHitboxOffsetY = 0;
constexpr int kPlayerHitboxWidth = 4;
constexpr int kPlayerHitboxHeight = 4;

constexpr int kPlayerOffsetX = 6;
constexpr int kPlayerOffsetY = 12;
constexpr int kPlayerFrameCount = 8;
constexpr int kPlayerFrameMod = 2;

constexpr float kPlayerShootingKnockback = 0.85f;
constexpr float kPlayerHurtKnockback = 4.0f;

Player::Player() {
	bx = kPlayerHitboxOffsetX;
	by = kPlayerHitboxOffsetY;
	bw = kPlayerHitboxWidth;
	bh = kPlayerHitboxHeight;
	state = PlayerState::Normal;
}

void Player::Hurt(const CoordXY<float> &direction) {
	speed = direction * kPlayerHurtKnockback;
}

void Player::Update() {
	HandleHorizontalMovement();
	HandleVerticalMovement();
}

void Player::Render() {
	auto frame = static_cast<int>(timer * kPlayerFrameCount) % kPlayerFrameMod;
	if (speed == CoordXY<float>::Zero) {
		frame = 0;
	}

	pti_spr(bitmap_player, frame, position.x - kPlayerOffsetX, position.y - kPlayerOffsetY, false, false);
}

void Player::HandleHorizontalMovement() {
	if (pti_down(PTI_Y)) {
		_pti_appr(speed.x, -kPlayerMaxSpeed, kPlayerAcceleration * PTI_DELTA);
	} else if (pti_down(PTI_A)) {
		_pti_appr(speed.x, kPlayerMaxSpeed, kPlayerAcceleration * PTI_DELTA);
	} else {
		_pti_appr(speed.x, 0, kPlayerFriction * PTI_DELTA);
	}
}

void Player::HandleVerticalMovement() {
	if (pti_down(PTI_X)) {
		_pti_appr(speed.y, -kPlayerMaxSpeed, kPlayerAcceleration * PTI_DELTA);
	} else if (pti_down(PTI_B)) {
		_pti_appr(speed.y, kPlayerMaxSpeed, kPlayerAcceleration * PTI_DELTA);
	} else {
		_pti_appr(speed.y, 0, kPlayerFriction * PTI_DELTA);
	}
}