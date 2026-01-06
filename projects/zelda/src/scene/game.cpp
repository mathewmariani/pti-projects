#include "pti/pti.h"

#include "batteries/actor.h"
#include "batteries/solid.h"
#include "batteries/assets.h"
#include "batteries/palettes.h"

#include "game.h"
#include "../bank.h"

#include "../gamestate.h"

#include "../entity/actor/player.h"

pti_palette_t pal = {
		.count = 16,
		.colors = &sweetie16[0],
};

void GameScene::Init(void) {
	batteries::init();
	bitmap_player = batteries::sprite("assets/link.ase");
	palette = batteries::palette("assets/sweetie16.hex");
	flags = batteries::flags("assets/flags.bin");
	tilemap = batteries::tilemap("assets/tilemap.ase");
	tileset = batteries::tileset("assets/tilemap.ase");

	pti_set_palette(palette);
	pti_set_flags(flags);
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

constexpr float kTransitionSpeed = 0.25f;

void GameScene::Render(void) {
	pti_cls(15);

	auto gameState = GetGameState();
	auto target = gameState.currentRoom * 128.0f;
	auto diff = (target - camera) * kTransitionSpeed;
	camera = camera + diff;

	pti_camera((int) std::round(camera.x), (int) std::round(camera.y));

	pti_map(0, 0);
	RenderEntitiesOfType<EntityBase>();
}