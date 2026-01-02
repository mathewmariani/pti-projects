#pragma once

#include "batteries/scene.h"

// actors
#include "../entity/actor/player.h"

// solids
#include "../entity/solid/platform.h"

using BasicScene = batteries::Scene<256, Player, Platform>;

struct GameScene : public BasicScene {
	void Init(void) override;
	void Update(void) override;
	void Render(void) override;
};