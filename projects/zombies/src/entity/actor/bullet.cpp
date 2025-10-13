#include "bullet.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

void Bullet::Update() {
}

void Bullet::Render() {
	auto frame = static_cast<int>(timer * kBulletFrameCount) % kBulletFrameMod;
	if (sx == 0 && grounded) {
		frame = 0;
	}

	pti_spr(bitmap_player, frame, x - kBulletOffsetX, y - kBulletOffsetY, false, false);
}