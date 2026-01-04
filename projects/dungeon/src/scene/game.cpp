#include "pti/pti.h"

#include "batteries/actor.h"
#include "batteries/solid.h"

#include "game.h"

#include "../entity/actor/player.h"

void GameScene::Init(void) {
	Reset();

	if (auto *e = CreateEntity<Player>(); e) {
		e->SetLocation({64, 64});
	}
}

void GameScene::Update(void) {
	UpdateEntitiesOfType<Solid>();
	UpdateEntitiesOfType<Actor>();
}

void GameScene::Render(void) {
	pti_cls(0xff575757);
	pti_map(0, 0);
	RenderEntitiesOfType<EntityBase>();
}