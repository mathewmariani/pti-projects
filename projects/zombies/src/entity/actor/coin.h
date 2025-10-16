#pragma once

#include "../actor.h"

constexpr int kCoinBoundaryOffset = 2;
constexpr int kCoinWidth = 4;
constexpr int kCoinHeight = 4;
constexpr int kCoinFrameCount = 8;
constexpr int kCoinFrameMod = 2;

struct Coin : Actor {
	Coin() {
		bx = 0;
		by = 0;
		bw = kCoinWidth;
		bh = kCoinHeight;
	}

	static void Create(const CoordXY<int> &location);

	void Update() override;
	void Render() override;
};