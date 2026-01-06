#include "pti/pti.h"

#include <memory>
#include <vector>

// batteries
#include "batteries/actor.h"
#include "batteries/solid.h"
#include "batteries/assets.h"

// actors
#include "../entity/actor/player.h"

// actors
#include "../entity/solid/platform.h"

#include "game.h"
#include "../bank.h"
#include "../gamestate.h"

// should be defined elsewhere
constexpr int kTileSize = 8;
constexpr float kDeathResetTimer = 2.0f;

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

void GameScene::Init(void) {
	batteries::init();
	palette = batteries::palette("assets/palette.hex");
	flags = batteries::flags("assets/flags.bin");
	tileset = batteries::tileset("assets/tilemap.ase");
	tilemap = batteries::tilemap("assets/tilemap.ase");
	bitmap_player = batteries::sprite("assets/dog.ase");
	bitmap_platform = batteries::sprite("assets/platform.ase");
	bitmap_font = batteries::sprite("assets/font.ase");

	pti_set_palette(palette);
	pti_set_flags(flags);
	pti_set_tilemap(tilemap);
	pti_set_tileset(tileset);
	pti_set_font(bitmap_font);

	batteries::reload();

	Reset();
	int i, j, t;
	for (i = 0; i < tilemap->height; i++) {
		for (j = 0; j < tilemap->width; j++) {
			t = pti_mget(i, j);
			switch (t) {
				case 48: {
					if (auto *e = CreateEntity<Player>(); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
				} break;
				case 51:
					if (auto *e = CreateEntity<Platform>(Platform::Type::Vertical); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
				case 52:
					if (auto *e = CreateEntity<Platform>(Platform::Type::Horizontal); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
			}
		}
	}
}

void GameScene::Update(void) {
	auto &gameState = GetGameState();
	if (gameState.PlayerIsDead) {
		gameState.ResetTimer += PTI_DELTA;
		if (gameState.ResetTimer >= kDeathResetTimer) {
			GameStateInit();
			return;
		}
	}

	UpdateEntitiesOfType<Solid>();
	UpdateEntitiesOfType<Actor>();
}

void GameScene::Render(void) {
	pti_cls(0);
	pti_map(0, 0);

	RenderEntitiesOfType<EntityBase>();
}