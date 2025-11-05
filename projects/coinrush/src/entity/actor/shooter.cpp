#include "pti/pti.h"

#include "shooter.h"
#include "effect.h"
#include "bullet.h"
#include "../../gamestate.h"
#include "../../bank.h"

constexpr float kShooterFireRate = 1.0f;
constexpr int kShooterOffsetX = 4;
constexpr int kShooterOffsetY = 8;
constexpr int kShooterWidth = 8;
constexpr int kShooterHeight = 8;

Shooter::Shooter(const CoordXY<int> &dir) {
	bx = 0;
	by = 0;
	bw = kShooterWidth;
	bh = kShooterHeight;

	shoot_direction = dir;
}

void Shooter::Update() {
	shoot_timer -= PTI_DELTA;
	if (shoot_timer < 0.0f) {
		shoot_timer = kShooterFireRate;
		if (auto *e = CreateEntity<Bullet>(); e) {
			auto loc = position + (CoordXY<int>::One * 4);
			e->SetLocation(loc);
			e->direction = shoot_direction;

			Effect::Create(loc, Effect::Type::Collect);
		}
	}
}

void Shooter::Render() {
	pti_spr(bitmap_shooter, 0, position.x - kShooterOffsetX, position.y - kShooterOffsetY, false, false);
}

void Shooter::HandleHorizontalMovement() {
	// nothing.
}

void Shooter::HandleVerticalMovement() {
	// nothing.
}