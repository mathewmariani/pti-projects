#pragma once

#include "batteries/scene.h"

// actors
#include "../entity/actor/bullet.h"
#include "../entity/actor/coin.h"
#include "../entity/actor/effect.h"
#include "../entity/actor/player.h"
#include "../entity/actor/zombie.h"

using BasicScene = batteries::Scene<256, Bullet, Coin, Effect, Player, Zombie>;

struct GameScene : public BasicScene {
	void Init(void) override;
	void Update(void) override;
	void Render(void) override;
};