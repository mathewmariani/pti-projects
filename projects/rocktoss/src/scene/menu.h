#pragma once

#include "batteries/scene.h"

#include "../entity/actor/player.h"

struct MenuScene : public batteries::Scene<256, Player> {
	void Init(void) override;
	void Update(void) override;
	void Render(void) override;
};