#include "pti/pti.h"
#include "coin.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"

void Coin::Create(const CoordXY<int> &location) {
	auto *coin = (Coin *) CreateEntity<Coin>();
	if (coin == nullptr) {
		return;
	}

	coin->bx = kCoinBoundaryOffset;
	coin->by = kCoinBoundaryOffset;
	coin->bw = kCoinWidth;
	coin->bh = kCoinHeight;

	coin->SetLocation(location);
}

void Coin::Update() {
	// nothing.
}

void Coin::Render() {
	auto frame = static_cast<int>(timer * kCoinFrameCount) % kCoinFrameMod;
	pti_spr(bitmap_coin, frame, x, y, false, false);
}