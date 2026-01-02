#pragma once

#include "batteries/scene.h"

// actors
#include "../entity/actor/bullet.h"
#include "../entity/actor/coin.h"
#include "../entity/actor/effect.h"
#include "../entity/actor/flag.h"
#include "../entity/actor/goomba.h"
#include "../entity/actor/player.h"

// solids
#include "../entity/solid/platform.h"
#include "../entity/solid/shooter.h"


using BasicScene = batteries::Scene<256, Bullet, Coin, Effect, Flag, Goomba, Player, Shooter, Platform>;

struct GameScene : public BasicScene {
	void Init(void) override;
	void Update(void) override;
	void Render(void) override;
};