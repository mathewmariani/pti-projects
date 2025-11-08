#include "pti/pti.h"
#include "coin.h"
#include "../../gamestate.h"
#include "../../bank.h"

constexpr int kCoinOffsetX = 4;
constexpr int kCoinOffsetY = 4;
constexpr int kCoinWidth = 8;
constexpr int kCoinHeight = 8;

constexpr int kCoinHitboxOffsetX = -4;
constexpr int kCoinHitboxOffsetY = -4;
constexpr int kCoinHitboxWidth = 8;
constexpr int kCoinHitboxHeight = 8;

constexpr int kCoinFrameCount = 8;
constexpr int kCoinFrameMod = 2;

void Coin::Create(const CoordXY<int> &location) {
	auto *coin = (Coin *) CreateEntity<Coin>();
	if (coin == nullptr) {
		return;
	}

	coin->SetLocation(location);
}

Coin::Coin() {
	bx = kCoinHitboxOffsetX;
	by = kCoinHitboxOffsetY;
	bw = kCoinHitboxWidth;
	bh = kCoinHitboxHeight;
}

void Coin::Update() {
	// nothing.
}

void Coin::Render() {
	auto frame = static_cast<int>(timer * kCoinFrameCount) % kCoinFrameMod;
	pti_spr(bitmap_coin, frame, position.x - kCoinOffsetX, position.y - kCoinOffsetY, false, false);
}