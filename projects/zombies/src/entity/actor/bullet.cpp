#include "bullet.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

void Bullet::Update() {
	_pti_appr(sx, direction.x * kBulletMaxSpeed, kBulletAcceleration * PTI_DELTA);
	_pti_appr(sy, direction.y * kBulletMaxSpeed, kBulletAcceleration * PTI_DELTA);

	if (PlaceMeeting(direction)) {
		RemoveEntity(this);
		return;
	}

	auto collision = false;
	for (auto *zombie : GetCollisions<Zombie>(*this, direction)) {
		zombie->Hurt(direction);
		collision = true;
	}

	if (collision) {
		Effect::Create({x, y}, Effect::Type::Collect);
		RemoveEntity(this);
	}
}

void Bullet::Render() {
	auto frame = static_cast<int>(timer * kBulletFrameCount) % kBulletFrameMod;
	pti_spr(bitmap_bullet, frame, x - kBulletOffsetX, y - kBulletOffsetY, false, false);
}