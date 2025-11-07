#include "pti/pti.h"

#include "shooter.h"
#include "../actor/effect.h"
#include "../actor/bullet.h"
#include "../../gamestate.h"
#include "../../bank.h"

constexpr float kShooterFireRate = 1.0f;

constexpr int kShooterHitboxOffsetX = -4;
constexpr int kShooterHitboxOffsetY = -4;
constexpr int kShooterHitboxWidth = 12;
constexpr int kShooterHitboxHeight = 12;

constexpr int kShooterOffsetX = 4;
constexpr int kShooterOffsetY = 4;
constexpr int kShooterWidth = 8;
constexpr int kShooterHeight = 8;


Shooter::Shooter(const CoordXY<int> &dir) {
	bx = kShooterHitboxOffsetX;
	by = kShooterHitboxOffsetY;
	bw = kShooterHitboxWidth;
	bh = kShooterHitboxHeight;

	shoot_direction = dir;
}

void Shooter::Update() {
	shoot_timer -= PTI_DELTA;
	if (shoot_timer < 0.0f) {
		shoot_timer = kShooterFireRate;
		if (auto *e = CreateEntity<Bullet>(); e) {
			auto loc = position;

			if (shoot_direction.x != 0) {
				loc = loc + (CoordXY<int>::Y * kShooterOffsetX);
			} else if (shoot_direction.y != 0) {
				loc = loc + (CoordXY<int>::X * kShooterOffsetY);
			}

			if (shoot_direction.x > 0) {
				loc = loc + (shoot_direction * 8);
			} else if (shoot_direction.y > 0) {
				loc = loc + (shoot_direction * 8);
			}

			loc = loc + shoot_direction;

			e->SetLocation(loc);
			e->direction = shoot_direction;

			Effect::Create(loc, Effect::Type::Collect);
		}
	}
}

void Shooter::Render() {
	auto flip_x = false;
	auto flip_y = false;
	auto frame = 0;
	if (shoot_direction.x != 0) {
		frame = 1;
		flip_x = (shoot_direction.x > 0);
	} else if (shoot_direction.y != 0) {
		flip_y = (shoot_direction.y < 0);
	}

	pti_spr(bitmap_shooter, frame, position.x - kShooterOffsetX, position.y - kShooterOffsetY, flip_x, flip_y);
}

void Shooter::HandleHorizontalMovement() {
	// nothing.
}

void Shooter::HandleVerticalMovement() {
	// nothing.
}