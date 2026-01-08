#pragma once

#include "batteries/scene.h"

// actors
#include "../entity/actor/crawler.h"

using BasicScene = batteries::Scene<256, Crawler>;

struct GameScene : public BasicScene {
	void Init(void) override;
	void Update(void) override;
	void Render(void) override;

	int GetTileSize(void) { return 2; };

private:
	CoordXY<float> camera = CoordXY<float>::Zero;
};