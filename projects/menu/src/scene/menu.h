#pragma once

#include "batteries/scene.h"

using BasicScene = batteries::Scene<256>;

struct MenuScene : public BasicScene {
	void Init(void) override;
	void Update(void) override;
	void Render(void) override;
};