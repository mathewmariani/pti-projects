#include "effect.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

constexpr int kEffectOffsetX = -4;
constexpr int kEffectOffsetY = -4;
constexpr int kEffectWidth = 0;
constexpr int kEffectHeight = 0;

constexpr int kEffectFrameCount = 8;
constexpr int kEffectFrameMod = 5;

void Effect::Create(const CoordXY<int> &location, Effect::Type type) {
	auto *effect = (Effect *) CreateEntity<Effect>();
	if (effect == nullptr) {
		return;
	}

	effect->SetLocation(location);
}

Effect::Effect() {
	bx = 0;
	by = 0;
	bw = 0;
	bh = 0;
	type = Type::Collect;
}

void Effect::Update() {
	auto frame = static_cast<int>(timer * kEffectFrameCount);
	if (frame == (kEffectFrameMod)) {
		Destroy();
	}
}

void Effect::Render() {
	auto frame = static_cast<int>(timer * kEffectFrameCount) % kEffectFrameMod;

	switch (type) {
		case Type::Collect:
			pti_spr(bitmap_fx_collect, frame, position.x + kEffectOffsetX, position.y + kEffectOffsetX, false, false);
			break;
	}
}