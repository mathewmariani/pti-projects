#include "player.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"
#include "bullet.h"

void Player::Hurt(const CoordXY<float> &direction) {
	sx = direction.x * kPlayerHurtKnockback;
	sy = direction.y * kPlayerHurtKnockback;
	health -= 1;

	if (health <= 0) {
		RemoveEntity(this);
	}
}

void Player::Update() {
	shoot_timer -= PTI_DELTA;

	auto dir = CoordXY<int>::Zero;
	if (pti_down(PTI_LEFT)) {
		dir = CoordXY<int>::Left;
	} else if (pti_down(PTI_RIGHT)) {
		dir = CoordXY<int>::Right;
	} else if (pti_down(PTI_UP)) {
		dir = CoordXY<int>::Down;
	} else if (pti_down(PTI_DOWN)) {
		dir = CoordXY<int>::Up;
	}

	if (dir != CoordXY<int>::Zero && shoot_timer < 0.0f) {
		shoot_timer = kPlayerFireRate;
		if (auto *e = CreateEntity<Bullet>(); e) {
			e->SetLocation({x, y});
			e->direction = dir;
			sx = dir.x * -kPlayerShootingKnockback;
			sy = dir.y * -kPlayerShootingKnockback;
		}
	}

	HandleHorizontalMovement();
	HandleVerticalMovement();

	// camera movement
	int cam_x, cam_y;
	pti_get_camera(&cam_x, &cam_y);
	cam_x += ((x + bx + bw / 2.0f - EN_ROOM_WIDTH / 2.0f) - cam_x) / 10;
	cam_y += ((y + by + bh / 2.0f - EN_ROOM_HEIGHT / 2.0f) - cam_y) / 10;
	pti_camera(cam_x, cam_y);
}

void Player::Render() {
	auto frame = static_cast<int>(timer * kPlayerFrameCount) % kPlayerFrameMod;
	if (sx == 0 && sy == 0) {
		frame = 0;
	}

	pti_spr(bitmap_player, frame, x - kPlayerOffsetX, y - kPlayerOffsetY, false, false);
}

void Player::HandleHorizontalMovement() {
	if (pti_down(PTI_Y)) {
		_pti_appr(sx, -kPlayerMaxSpeed, kPlayerAcceleration * PTI_DELTA);
	} else if (pti_down(PTI_A)) {
		_pti_appr(sx, kPlayerMaxSpeed, kPlayerAcceleration * PTI_DELTA);
	} else {
		_pti_appr(sx, 0, kPlayerFriction * PTI_DELTA);
	}
}

void Player::HandleVerticalMovement() {
	if (pti_down(PTI_X)) {
		_pti_appr(sy, -kPlayerMaxSpeed, kPlayerAcceleration * PTI_DELTA);
	} else if (pti_down(PTI_B)) {
		_pti_appr(sy, kPlayerMaxSpeed, kPlayerAcceleration * PTI_DELTA);
	} else {
		_pti_appr(sy, 0, kPlayerFriction * PTI_DELTA);
	}
}