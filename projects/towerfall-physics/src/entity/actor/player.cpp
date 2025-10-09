#include "player.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

template<>
bool EntityBase::Is<Player>() const {
	return type == EntityType::Player;
}

void Player::Update() {
	HandleHorizontalMovement();
	HandleVerticalMovement();
	HandleJump();

	// camera movement
	int cam_x, cam_y;
	pti_get_camera(&cam_x, &cam_y);
	cam_x += ((x + bx + bw / 2.0f - EN_ROOM_WIDTH / 2.0f) - cam_x) / 10;
	cam_y += ((y + by + bh / 2.0f - EN_ROOM_HEIGHT / 2.0f) - cam_y) / 10;
	pti_camera(cam_x, cam_y);
}

void Player::Render() {
	auto frame = static_cast<int>(timer * kPlayerFrameCount) % kPlayerFrameMod;
	if (sx == 0 && IsGrounded()) {
		frame = 0;
	}

	pti_spr(bitmap_player, frame, x - kPlayerOffsetX, y - kPlayerOffsetY, false, false);
}

void Player::HandleHorizontalMovement() {
	if (pti_down(PTI_LEFT)) {
		_pti_appr(sx, -kPlayerMaxSpeed, kPlayerAcceleration * PTI_DELTA);
	} else if (pti_down(PTI_RIGHT)) {
		_pti_appr(sx, kPlayerMaxSpeed, kPlayerAcceleration * PTI_DELTA);
	} else {
		_pti_appr(sx, 0, kPlayerFriction * PTI_DELTA);
	}
}

void Player::HandleVerticalMovement() {
	static int hang_time = 0;

	if (IsGrounded() && state == PlayerState::Jump) {
		if (sy <= -0.5f) {
			hang_time = 3;
			sy += kPlayerPhysicsVerticalGravFall;
		} else {
			if (hang_time > 0) {
				--hang_time;
				sy = 0;
			} else {
				sy += kPlayerPhysicsVerticalGravFall;
			}
		}
	} else {
		sy += kPlayerPhysicsVerticalGravFall;
	}

	// Limit vertical speed
	if (sy > kPlayerPhysicsVerticalMax) {
		sy = kPlayerPhysicsVerticalMax;
	}
}

void Player::HandleJump() {
	auto kJump = pti_pressed(PTI_UP);
	auto grounded = IsGrounded();

	// Revert state
	if (state == PlayerState::Jump && grounded) {
		state = PlayerState::Normal;
	}

	// Full jump
	if (kJump && grounded) {
		state = PlayerState::Jump;
		sy = -kPlayerPhysicsJumpStrength;
	}

	// Variable jump
	if (state == PlayerState::Jump && sy < -(kPlayerPhysicsJumpStrength * 0.5f)) {
		if (pti_released(PTI_UP)) {
			sy = -(kPlayerPhysicsJumpStrength * 0.5f);
		}
	}
}