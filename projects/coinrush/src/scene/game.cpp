#include "pti/pti.h"

#include <memory>
#include <vector>

// batteries
#include "batteries/actor.h"
#include "batteries/solid.h"
#include "batteries/assets.h"
#include "batteries/helper.h"

// actors
#include "../entity/actor/bullet.h"
#include "../entity/actor/coin.h"
#include "../entity/actor/effect.h"
#include "../entity/actor/flag.h"
#include "../entity/actor/goomba.h"
#include "../entity/actor/player.h"

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
	bitmap_bullet = batteries::sprite("assets/bullet.ase");
	bitmap_coin = batteries::sprite("assets/coin.ase");
	bitmap_player = batteries::sprite("assets/dog.ase");
	bitmap_goomba = batteries::sprite("assets/zombie.ase");
	bitmap_heart = batteries::sprite("assets/heart.ase");
	bitmap_platform = batteries::sprite("assets/platform.ase");
	bitmap_font = batteries::sprite("assets/font.ase");
	bitmap_fx_collect1 = batteries::sprite("assets/collect.ase");
	bitmap_fx_collect2 = batteries::sprite("assets/collect2.ase");
	bitmap_shooter = batteries::sprite("assets/cannon.ase");
	bitmap_door = batteries::sprite("assets/door.ase");

	GetGameState().levels = {
			batteries::tilemap("assets/levels/01.ase"),
			batteries::tilemap("assets/levels/02.ase"),
			batteries::tilemap("assets/levels/03.ase"),
			batteries::tilemap("assets/levels/04.ase"),
			batteries::tilemap("assets/levels/05.ase"),
			batteries::tilemap("assets/levels/06.ase"),
	};

	pti_set_palette(palette);
	pti_set_flags(flags);
	pti_set_tileset(tileset);
	pti_set_font(bitmap_font);

	batteries::reload();

	Reset();

	auto &levels = GetGameState().levels;
	auto next = -1;
	do {
		next = RandomRange(0, levels.size() - 1);
	} while (next == GetGameState().CurrentLevelIndex);

	pti_set_tilemap(levels[next]);

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
				case 49:
					Coin::Create({XPOS(i), YPOS(j)});
					pti_mset(i, j, 0);
					break;
				case 50: {
					Goomba::Create({XPOS(i), YPOS(j)});
					pti_mset(i, j, 0);
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
					// shooter
				case 57:
					if (auto *e = CreateEntity<Shooter>(CoordXY<int>::Down); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
				case 58:
					if (auto *e = CreateEntity<Shooter>(CoordXY<int>::Up); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
				case 59:
					if (auto *e = CreateEntity<Shooter>(CoordXY<int>::Right); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
				case 60:
					if (auto *e = CreateEntity<Shooter>(CoordXY<int>::Left); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;

				case 61:
					if (auto *e = CreateEntity<Flag>(); e) {
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
			gameState.Deaths++;
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