#include "pti/pti.h"
#include "flag.h"
#include "../../gamestate.h"
#include "../../bank.h"

constexpr int kFlagHitboxOffsetX = 0;
constexpr int kFlagHitboxOffsetY = 0;
constexpr int kFlagHitboxWidth = 8;
constexpr int kFlagHitboxHeight = 8;

constexpr int kFlagOffsetX = -8;
constexpr int kFlagOffsetY = -8;
constexpr int kFlagWidth = 16;
constexpr int kFlagHeight = 16;

constexpr int kFlagFrameCount = 5;
constexpr int kFlagFrameMod = 2;

void Flag::Create(const CoordXY<int> &location) {
	auto *flag = (Flag *) CreateEntity<Flag>();
	if (flag == nullptr) {
		return;
	}

	flag->SetLocation(location);
}

Flag::Flag() {
	bx = kFlagHitboxOffsetX;
	by = kFlagHitboxOffsetY;
	bw = kFlagHitboxWidth;
	bh = kFlagHitboxHeight;
}

void Flag::Update() {
	auto collision = false;
	for (auto *player : GetCollisions<Player>(this, direction)) {
		collision = true;
	}

	if (collision) {
		GameStateInit();
	}
}

void Flag::Render() {
	auto frame = static_cast<int>(timer * kFlagFrameCount) % kFlagFrameMod;
	pti_spr(bitmap_door, 0, position.x + kFlagOffsetX, position.y + kFlagOffsetX, false, false);
}