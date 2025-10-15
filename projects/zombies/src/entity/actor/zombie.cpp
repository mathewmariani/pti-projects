#include "zombie.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

void Zombie::Hurt(const CoordXY<int> &direction) {
	sx = direction.x * kZombieKnockback;
	sy = direction.y * kZombieKnockback;
	health -= 1;

	if (health <= 0) {
		RemoveEntity(this);
	}
}

void Zombie::Update() {
	_pti_appr(sx, 0.0f, kZombieFriction * PTI_DELTA);
	_pti_appr(sy, 0.0f, kZombieFriction * PTI_DELTA);
}

void Zombie::Render() {
	auto frame = static_cast<int>(timer * kZombieFrameCount) % kZombieFrameMod;
	if (sx == 0 && sy == 0) {
		frame = 0;
	}

	pti_spr(bitmap_zombie, frame, x - kZombieOffsetX, y - kZombieOffsetY, false, false);
}