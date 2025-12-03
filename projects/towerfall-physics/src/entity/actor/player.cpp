#include "player.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

#include "batteries/juice.h"

constexpr float kPlayerMaxSpeed = 3.0f;
constexpr float kPlayerAcceleration = 20.0f;
constexpr float kPlayerFriction = 15.0f;
constexpr float kPlayerPhysicsVerticalMax = 6.0f;
constexpr float kPlayerPhysicsVerticalGrav = 0.24f;
constexpr float kPlayerPhysicsVerticalGravFall = 0.5f;
constexpr float kPlayerPhysicsVerticalGravAlt = 0.1940f;
constexpr float kPlayerPhysicsJumpStrength = 4.7397f;
constexpr float kPlayerPhysicsBounceStrength = 4.5535f;

constexpr float kPlayerJumpBuffer = 0.1334f;
constexpr float kPlayerCoyoteTime = 0.1;

constexpr int kPlayerHitboxOffsetX = -4;
constexpr int kPlayerHitboxOffsetY = -4;
constexpr int kPlayerHitboxWidth = 8;
constexpr int kPlayerHitboxHeight = 8;

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

void Player::Squish() {
	RemoveEntity(this);
	GetGameState().PlayerIsDead = true;
}

void Player::Update() {
	HandleHorizontalMovement();
	HandleVerticalMovement();
	HandleJump();
}

void Player::Render() {
	auto frame = static_cast<int>(timer * kPlayerFrameCount) % kPlayerFrameMod;
	if (speed.x == 0 && grounded) {
		frame = 0;
	}

	pti_spr(bitmap_player, frame, position.x - kPlayerOffsetX, position.y - kPlayerOffsetY, false, false);
}

void Player::HandleHorizontalMovement() {
	if (pti_down(PTI_LEFT)) {
		_pti_appr(speed.x, -kPlayerMaxSpeed, kPlayerAcceleration * PTI_DELTA);
	} else if (pti_down(PTI_RIGHT)) {
		_pti_appr(speed.x, kPlayerMaxSpeed, kPlayerAcceleration * PTI_DELTA);
	} else {
		_pti_appr(speed.x, 0, kPlayerFriction * PTI_DELTA);
	}
}

void Player::HandleVerticalMovement() {
	static int hang_time = 0;

	float grav = kPlayerPhysicsVerticalGravFall;

	if (!grounded && coyoteTime > 0.0f) {
		grav *= 0.5f;
	}

	bool in_jump = (!grounded && state == PlayerState::Jump);
	if (in_jump) {
		if (speed.y <= -0.5f) {
			hang_time = 3;
			speed.y += grav;
		} else if (hang_time > 0) {
			--hang_time;
			speed.y = 0;
		} else {
			speed.y += grav;
		}
	} else if (!grounded) {
		speed.y += grav;
	} else {
		speed.y = 0.0f;
	}

	speed.y = std::min(speed.y, kPlayerPhysicsVerticalMax);
}


void Player::HandleJump() {
	bool kJumpPressed = pti_pressed(PTI_UP);
	bool kJumpReleased = pti_released(PTI_UP);

	if (kJumpPressed) {
		jumpBuffer = kPlayerJumpBuffer;
	} else if (jumpBuffer > 0.0f) {
		jumpBuffer -= PTI_DELTA;
		if (jumpBuffer < 0.0f) jumpBuffer = 0.0f;
	}

	if (grounded) {
		coyoteTime = kPlayerCoyoteTime;
	} else if (coyoteTime > 0.0f) {
		coyoteTime -= PTI_DELTA;
		if (coyoteTime < 0.0f) coyoteTime = 0.0f;
	}

	if (state == PlayerState::Jump && grounded) {
		state = PlayerState::Normal;
	}

	if ((kJumpPressed || jumpBuffer > 0.0f) && (grounded || coyoteTime > 0.0f)) {
		state = PlayerState::Jump;
		speed.y = -kPlayerPhysicsJumpStrength;
		jumpBuffer = 0.0f;
		coyoteTime = 0.0f;
	}

	if (state == PlayerState::Jump && speed.y < -(kPlayerPhysicsJumpStrength * 0.5f)) {
		if (kJumpReleased) {
			speed.y = -(kPlayerPhysicsJumpStrength * 0.5f);
		}
	}
}
