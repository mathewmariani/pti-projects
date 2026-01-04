#include "pti/pti.h"

#include "batteries/actor.h"
#include "batteries/solid.h"
#include "batteries/assets.h"

#include "game.h"
#include "../bank.h"

#include "../entity/actor/player.h"

void GameScene::Init(void) {
	batteries::init();
	bitmap_player = batteries::sprite("assets/link.ase");
	tilemap = batteries::tilemap("assets/tilemap.ase");
	tileset = batteries::tileset("assets/tilemap.ase");

	pti_set_tilemap(tilemap);
	pti_set_tileset(tileset);

	// reload loads the specific bank into pti
	batteries::reload();

	Reset();

	if (auto *e = CreateEntity<Player>(); e) {
		e->SetLocation({56, 56});
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