#include "zombie.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

void Zombie::Update() {
}

void Zombie::Render() {
	auto frame = static_cast<int>(timer * kZombieFrameCount) % kZombieFrameMod;
	if (sx == 0 && grounded) {
		frame = 0;
	}

	pti_spr(bitmap_player, frame, x - kZombieOffsetX, y - kZombieOffsetY, false, false);
}