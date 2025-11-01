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

Shooter::Shooter() {
	bx = 0;
	by = 0;
	bw = kShooterWidth;
	bh = kShooterHeight;
	direction = {-1, 0};
}

void Shooter::Update() {
	shoot_timer -= PTI_DELTA;
	if (shoot_timer < 0.0f) {
		shoot_timer = kShooterFireRate;
		Bullet::Create({position.x - 8, position.y});
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