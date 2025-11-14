#include "bullet.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

constexpr float kBulletMaxSpeed = 6.25f;
constexpr int kBulletBoundaryOffset = 2;

constexpr int kBulletOffsetX = 4;
constexpr int kBulletOffsetY = 4;
constexpr int kBulletWidth = 8;
constexpr int kBulletHeight = 8;

constexpr int kBulletHitboxOffsetX = -4;
constexpr int kBulletHitboxOffsetY = -4;
constexpr int kBulletHitboxWidth = 8;
constexpr int kBulletHitboxHeight = 8;
constexpr int kBulletFrameCount = 1;
constexpr int kBulletFrameMod = 1;

Bullet::Bullet() {
	bx = kBulletHitboxOffsetX;
	by = kBulletHitboxOffsetY;
	bw = kBulletWidth;
	bh = kBulletHeight;
	direction = CoordXY<int>::Right;
}

void Bullet::Update() {
	speed = direction * kBulletMaxSpeed;

	auto collision = false;
	for (auto *zombie : GetCollisions<Zombie>(this, direction)) {
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