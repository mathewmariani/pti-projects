#include "pti/pti.h"
#include "flag.h"
#include "../../gamestate.h"
#include "../../bank.h"

constexpr int kBulletHitboxOffsetX = 0;
constexpr int kBulletHitboxOffsetY = 0;
constexpr int kBulletHitboxWidth = 8;
constexpr int kBulletHitboxHeight = 8;

constexpr int kFlagOffsetX = 0;
constexpr int kFlagOffsetY = 0;
constexpr int kFlagWidth = 8;
constexpr int kFlagHeight = 8;

constexpr int kFlagFrameCount = 8;
constexpr int kFlagFrameMod = 5;

void Flag::Create(const CoordXY<int> &location) {
	auto *flag = (Flag *) CreateEntity<Flag>();
	if (flag == nullptr) {
		return;
	}

	flag->SetLocation(location);
}

Flag::Flag() {
	bx = kBulletHitboxOffsetX;
	by = kBulletHitboxOffsetY;
	bw = kBulletHitboxWidth;
	bh = kBulletHitboxHeight;
}

void Flag::Update() {
	auto collision = false;
	for (auto *player : GetCollisions<Player>(this, direction)) {
		collision = true;
	}

	if (collision) {
		printf("Change level!\n");
	}
}

void Flag::Render() {
	auto frame = static_cast<int>(timer * kFlagFrameCount) % kFlagFrameMod;
	pti_rectf(position.x + kFlagOffsetX, position.y + kFlagOffsetX, kFlagWidth, kFlagHeight, 0xff00f0f0);
}