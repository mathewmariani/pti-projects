#include "player.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"
#include "bullet.h"

void Player::Hurt(const CoordXY<float> &direction) {
	speed = direction * kPlayerHurtKnockback;
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
			e->SetLocation(position);
			e->direction = dir;
			speed = dir * -kPlayerShootingKnockback;
		}
	}

	HandleHorizontalMovement();
	HandleVerticalMovement();

	// collect coins
	for (auto *coin : GetCollisions<Coin>(*this, direction)) {
		RemoveEntity(coin);
		GetGameState().Coins++;
	}

	// camera movement
	int cam_x, cam_y;
	pti_get_camera(&cam_x, &cam_y);
	cam_x += ((position.x + bx + bw / 2.0f - kScreenWidth / 2.0f) - cam_x) / 10;
	cam_y += ((position.y + by + bh / 2.0f - kScreenHeight / 2.0f) - cam_y) / 10;
	pti_camera(cam_x, cam_y);
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