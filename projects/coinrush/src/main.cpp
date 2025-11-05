/* engine */
#include "pti/pti.h"

#include "bank.h"
#include "gamestate.h"
#include "batteries/assets.h"
#include "batteries/registry.h"
#include "batteries/math.h"
#include "batteries/juice.h"

#include <string>

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

static void load(void) {
	GameStateInit();
	batteries::reload();

	pti_set_tilemap(levels[RandomRange(0, levels.size() - 1)]);

	int i, j, t;
	for (i = 0; i < EN_ROOM_COLS; i++) {
		for (j = 0; j < EN_ROOM_ROWS; j++) {
			t = pti_mget(i, j);
			switch (t) {
				case 48: {
					if (auto *e = CreateEntity<Player>(); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
						GetGameState().player = static_cast<Player *>(e);
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

static void init(void) {
	batteries::init();
	tileset = batteries::tileset("assets/tilemap.ase");
	tilemap = batteries::tilemap("assets/tilemap.ase");
	bitmap_bullet = batteries::sprite("assets/bullet.ase");
	bitmap_coin = batteries::sprite("assets/coin.ase");
	bitmap_player = batteries::sprite("assets/dog.ase");
	bitmap_goomba = batteries::sprite("assets/zombie.ase");
	bitmap_heart = batteries::sprite("assets/heart.ase");
	bitmap_platform = batteries::sprite("assets/platform.ase");
	bitmap_font = batteries::sprite("assets/font.ase");
	bitmap_fx_collect = batteries::sprite("assets/collect.ase");
	bitmap_shooter = batteries::sprite("assets/cannon.ase");

	levels = {
			batteries::tilemap("assets/levels/01.ase"),
			batteries::tilemap("assets/levels/02.ase"),
			batteries::tilemap("assets/levels/03.ase"),
			batteries::tilemap("assets/levels/04.ase"),
	};

	pti_set_tileset(tileset);
	pti_set_font(bitmap_font);

	load();

	/* graphics state: */
	// pti_dither(0x5a5a);
	pti_clip(0, 0, kScreenWidth, kScreenHeight);
}

static void cleanup(void) {
	/* ... */
}

static void frame(void) {
	auto &gameState = GetGameState();
	if (pti_down(PTI_DBG)) {
		GameStateReset();
		load();
		return;
	}

	if (gameState.PlayerIsDead) {
		gameState.ResetTimer += PTI_DELTA;
		if (gameState.ResetTimer >= kDeathResetTimer) {
			gameState.Deaths++;
			GameStateReset();
			load();
			return;
		}
	}

	GameStateTick();

	pti_cls(0xffef7d57);
	pti_map(0, 0);

	RenderAllEntities();

	DoShake();

	/* render ui */
	char buffer[100];

	// coin counter
	std::snprintf(buffer, sizeof(buffer), "Coins: %d\n", gameState.Coins);
	std::string coin_str(buffer);
	pti_print(coin_str.c_str(), 8, 4);

	// death counter
	std::snprintf(buffer, sizeof(buffer), "Deaths: %d\n", gameState.Deaths);
	std::string death_str(buffer);
	pti_print(death_str.c_str(), 8, 16);

	// debugging:
	// for (auto *e : GetEntitiesOfType<Actor>()) {
	// 	pti_rect(e->position.x + e->bx, e->position.y + e->by, e->bw - 1, e->bh - 1, 0xff00ff00);
	// }
}

pti_desc pti_main(int argc, char *argv[]) {

	return (pti_desc) {
			.init_cb = init,
			.frame_cb = frame,
			.cleanup_cb = cleanup,
			.memory_size = _pti_kilobytes(512), /* 256KB */
			.width = kScreenWidth,
			.height = kScreenHeight,
	};
}