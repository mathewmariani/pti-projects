#include "player.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

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
	state = Player::State::Normal;
}

void Player::Hurt(const CoordXY<float> &direction) {
	Effect::Create(this->position);

	speed = direction * kPlayerHurtKnockback;
	GetGameState().PlayerIsDead = true;
	RemoveEntity(this);
}

void Player::Update() {
	HandleHorizontalMovement();
	HandleVerticalMovement();
	HandleJump();

	// check for spikes
	if (PlaceMeeting(direction, 42) || PlaceMeeting(direction, 43) || PlaceMeeting(direction, 53) || PlaceMeeting(direction, 54)) {
		Hurt(CoordXY<float>::Zero);
		return;
	}

	// collect coins
	for (auto *coin : GetCollisions<Coin>(this, direction)) {
		Effect::Create(coin->position);
		RemoveEntity(coin);
		GetGameState().Coins++;
	}
}

void Player::Render() {
	auto frame = static_cast<int>(timer * kPlayerFrameCount) % kPlayerFrameMod;
	if (speed == CoordXY<float>::Zero) {
		frame = 0;
	}

	pti_spr(bitmap_player, frame, position.x - kPlayerOffsetX, position.y - kPlayerOffsetY, false, false);

	char buffer[100];
	std::snprintf(buffer, sizeof(buffer), "Is Dead: %f\n", speed.y);
	std::string status_str(buffer);

	pti_print(status_str.c_str(), 4, 10);
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
	if (!grounded) {
		if (speed.y > 0) {
			speed.y += kPlayerPhysicsVerticalGravFall * 1.5f;
		} else {
			speed.y += kPlayerPhysicsVerticalGravFall;
		}
	} else {
		speed.y = 0.0f;
	}

	// Limit vertical speed
	if (speed.y > kPlayerPhysicsVerticalMax) {
		speed.y = kPlayerPhysicsVerticalMax;
	}
}

void Player::HandleJump() {
	bool kJumpPressed = pti_pressed(PTI_UP);
	bool kJumpReleased = pti_released(PTI_UP);

	// jump buffer
	if (kJumpPressed) {
		jumpBuffer = kPlayerJumpBuffer;
	} else if (jumpBuffer > 0.0f) {
		jumpBuffer -= PTI_DELTA;
		if (jumpBuffer < 0.0f) jumpBuffer = 0.0f;
	}

	// coyote time
	if (grounded) {
		coyoteTime = kPlayerCoyoteTime;
	} else if (coyoteTime > 0.0f) {
		coyoteTime -= PTI_DELTA;
		if (coyoteTime < 0.0f) {
			coyoteTime = 0.0f;
		}
	}

	if (state == Player::State::Jump && grounded) {
		state = Player::State::Normal;
	}

	if ((kJumpPressed || jumpBuffer > 0.0f) && (grounded || coyoteTime > 0.0f)) {
		state = Player::State::Jump;
		speed.y = -kPlayerPhysicsJumpStrength;
		jumpBuffer = 0.0f;
		coyoteTime = 0.0f;
	}

	// short jump
	if (state == Player::State::Jump && speed.y < -(kPlayerPhysicsJumpStrength * 0.5f)) {
		if (kJumpReleased) {
			speed.y = -(kPlayerPhysicsJumpStrength * 0.5f);
		}
	}
}
