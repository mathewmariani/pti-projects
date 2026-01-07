#pragma once

#include "batteries/scene.h"

#include "../entity/actor/player.h"
#include "../entity/actor/rock.h"

using BasicScene = batteries::Scene<256, Player, Rock>;

struct GameScene : public BasicScene {
	void Init(void) override;
	void Update(void) override;
	void Render(void) override;

	int GetTileSize(void) override { return 8; };
};