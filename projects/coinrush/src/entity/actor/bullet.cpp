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

void Bullet::HaltX(void) {
	Effect::Create(position, Effect::Type::Collect);
	Destroy();
}

void Bullet::HaltY(void) {
	Effect::Create(position, Effect::Type::Collect);
	Destroy();
}

void Bullet::Update() {
	if (PlaceMeeting(direction) || CollidesWithSolids(direction)) {
		Effect::Create(position, Effect::Type::Collect);
		Destroy();
	}

	speed = direction * kBulletMaxSpeed;
	for (auto *player : GetCollisions<Player>(this, direction)) {
		player->Hurt(CoordXY<float>::Zero);
	}
}

void Bullet::Render() {
	auto frame = static_cast<int>(timer * kBulletFrameCount) % kBulletFrameMod;
	pti_spr(bitmap_bullet, frame, position.x - kBulletOffsetX, position.y - kBulletOffsetY, false, false);
}