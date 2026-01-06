/* engine */
#include "pti/pti.h"

#include "bank.h"
#include "gamestate.h"
#include "batteries/assets.h"
#include "batteries/palettes.h"

#include "entity/coin.h"
#include "entity/goomba.h"
#include "entity/player.h"
#include "entity/shooter.h"
#include "entity/registry.h"

#include <math.h>

#include <string>

namespace {
	constexpr int width = 240;
	constexpr int height = 135;
	auto t = 0.0f;

	constexpr int EN_ROOM_COLS = 64;
	constexpr int EN_ROOM_ROWS = 46;
	constexpr int EN_GRID_SIZE = 8;
	constexpr int EN_ROOM_WIDTH = 512;
	constexpr int EN_ROOM_HEIGHT = 288;

	float resetTimer = 0.0f;
}// namespace

#define XPOS(x) (x * EN_GRID_SIZE)
#define YPOS(y) (y * EN_GRID_SIZE)

pti_palette_t pal = {
		.count = 16,
		.colors = &sweetie16[0],
};

static void load(void) {
	GameStateInit();
	batteries::reload();

	int i, j, t;
	for (i = 0; i < EN_ROOM_COLS; i++) {
		for (j = 0; j < EN_ROOM_ROWS; j++) {
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
				case 51: {
					if (auto *e = CreateEntity<Shooter>(); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
				} break;
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
	bitmap_goomba = batteries::sprite("assets/goomba.ase");
	bitmap_player = batteries::sprite("assets/dog.ase");
	bitmap_shooter = batteries::sprite("assets/goomba.ase");
	bitmap_font = batteries::sprite("assets/font.ase");

	bitmap_fx_collect = batteries::sprite("assets/collect.ase");
	bitmap_fx_dust1 = batteries::sprite("assets/dust1.ase");
	bitmap_fx_dust2 = batteries::sprite("assets/dust2.ase");

	pti_set_palette(&pal);
	pti_set_tilemap(tilemap);
	pti_set_tileset(tileset);
	pti_set_font(bitmap_font);

	load();
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
			GameStateReset();
			load();
			return;
		}
	}

	GameStateTick();

	pti_cls(15);

	/* adjust camera */
	int cam_x, cam_y;
	pti_get_camera(&cam_x, &cam_y);
	pti_camera(_pti_max(0, _pti_min(EN_ROOM_WIDTH - width, cam_x)),
			   _pti_max(0, _pti_min(EN_ROOM_HEIGHT - height, cam_y)));

	pti_map(0, 0);
	RenderAllEntities();

	/* render ui */
	// const auto coin_str = std::format("coins: &d\n", coins);
	char buffer[100];
	std::snprintf(buffer, sizeof(buffer), "coins: %d\n", gameState.Coins);
	std::string coin_str(buffer);

	pti_color(12);
	pti_print(coin_str.c_str(), 4, 0);
}

pti_desc pti_main(int argc, char *argv[]) {

	return (pti_desc) {
			.init_cb = init,
			.frame_cb = frame,
			.cleanup_cb = cleanup,
			.memory_size = _pti_kilobytes(256), /* 256KB */
			.width = 240,
			.height = 135,
	};
}