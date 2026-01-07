#pragma once

#include "batteries/scene.h"

// actors
#include "../entity/actor/player.h"

using BasicScene = batteries::Scene<256, Player>;

struct GameScene : public BasicScene {
	void Init(void) override;
	void Update(void) override;
	void Render(void) override;

	int GetTileSize(void) override { return 16; };

private:
	CoordXY<float> camera = CoordXY<float>::Zero;
};