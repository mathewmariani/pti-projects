#include "bullet.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

void Bullet::Update() {
	speed = direction * kBulletMaxSpeed;

	auto collision = false;
	for (auto *zombie : GetCollisions<Zombie>(*this, direction)) {
		zombie->Hurt(direction);
		collision = true;
	}

	if (collision || PlaceMeeting(direction)) {
		Effect::Create(position, Effect::Type::Collect);
		RemoveEntity(this);
	}
}

void Bullet::Render() {
	auto frame = static_cast<int>(timer * kBulletFrameCount) % kBulletFrameMod;
	pti_spr(bitmap_bullet, frame, position.x - kBulletOffsetX, position.y - kBulletOffsetY, false, false);
}