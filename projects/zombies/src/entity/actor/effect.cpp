#include "pti/pti.h"
#include "effect.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"

void Effect::Create(const CoordXY<int> &location, Effect::Type type) {
	auto *effect = (Effect *) CreateEntity<Effect>();
	if (effect == nullptr) {
		return;
	}

	effect->bx = kEffectBoundaryOffset;
	effect->by = kEffectBoundaryOffset;
	effect->bw = kEffectWidth;
	effect->bh = kEffectHeight;
	effect->type = type;

	effect->SetLocation(location);
}

void Effect::Update() {
	auto frame = static_cast<int>(timer * kEffectFrameCount);
	if (frame == (kEffectFrameMod)) {
		RemoveEntity(this);
	}
}

void Effect::Render() {
	auto frame = static_cast<int>(timer * kEffectFrameCount) % kEffectFrameMod;

	switch (type) {
		case Type::Collect:
			pti_spr(bitmap_fx_collect, frame, x, y, false, false);
			break;
	}
}