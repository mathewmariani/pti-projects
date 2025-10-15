#pragma once

#include "../actor.h"

constexpr int kEffectBoundaryOffset = 2;
constexpr int kEffectWidth = 4;
constexpr int kEffectHeight = 4;
constexpr int kEffectFrameCount = 8;
constexpr int kEffectFrameMod = 5;

struct Effect : Actor {
	enum class Type : uint8_t {
		Collect,
	};

	Effect() {
		bx = 0;
		by = 0;
		bw = kEffectWidth;
		bh = kEffectHeight;
	}

	static void Create(const CoordXY<int> &location, Type type = Type::Collect);

	void Update() override;
	void Render() override;

	Type type;
};