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

constexpr int kBulletHitboxOffsetX = -2;
constexpr int kBulletHitboxOffsetY = -2;
constexpr int kBulletHitboxWidth = 4;
constexpr int kBulletHitboxHeight = 4;
constexpr int kBulletFrameCount = 1;
constexpr int kBulletFrameMod = 1;

void Bullet::Create(const CoordXY<int> &location) {
	auto *bullet = (Bullet *) CreateEntity<Bullet>();
	if (bullet == nullptr) {
		return;
	}

	bullet->SetLocation(location);
}

Bullet::Bullet() {
	bx = kBulletHitboxOffsetX;
	by = kBulletHitboxOffsetY;
	bw = kBulletHitboxWidth;
	bh = kBulletHitboxHeight;
}

void Bullet::Update() {
	speed = direction * kBulletMaxSpeed;
	if (PlaceMeeting(direction)) {
		Effect::Create(position, Effect::Type::Collect);
		RemoveEntity(this);
	}
}

void Bullet::Render() {
	auto frame = static_cast<int>(timer * kBulletFrameCount) % kBulletFrameMod;
	pti_spr(bitmap_bullet, frame, position.x - kBulletOffsetX, position.y - kBulletOffsetY, false, false);
}