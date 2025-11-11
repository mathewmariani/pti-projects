/* engine */
#include "pti/pti.h"

#include "bank.h"
#include "gamestate.h"
#include "batteries/assets.h"

/* actors */
#include "entity/actor/bullet.h"
#include "entity/actor/player.h"
#include "entity/actor/zombie.h"

#include <math.h>
#include <string>

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

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
						GetGameState().player = static_cast<Player *>(e);

						auto cam_x = e->position.x - kScreenWidth / 2.0f;
						auto cam_y = e->position.y - kScreenHeight / 2.0f;
						pti_camera(cam_x, cam_y);
					}
				} break;
				case 50: {
					if (auto *e = CreateEntity<Zombie>(); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
				} break;
			}
		}
	}

	for (auto i = 0; i < 200; i++) {
		auto location = RandomOutsideCamera();
		if (auto *e = CreateEntity<Zombie>(); e) {
			e->SetLocation(location);
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
	bitmap_zombie = batteries::sprite("assets/zombie.ase");
	bitmap_heart = batteries::sprite("assets/heart.ase");
	bitmap_platform = batteries::sprite("assets/platform.ase");
	bitmap_font = batteries::sprite("assets/font.ase");
	bitmap_fx_collect = batteries::sprite("assets/collect.ase");

	pti_set_tilemap(tilemap);
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
			GameStateReset();
			load();
			return;
		}
	}

	GameStateTick();

	for (auto *zombie : GetEntitiesOfType<Zombie>()) {
		if (IsWithinSpawnZone(zombie->position)) {
			continue;
		}

		zombie->position = RandomOutsideCamera();
	}

	pti_cls(0xffef7d57);

	// keep camera inbounds
	int cam_x, cam_y;
	pti_get_camera(&cam_x, &cam_y);
	int cx = _pti_max(0, _pti_min(EN_ROOM_WIDTH - kScreenWidth, cam_x));
	int cy = _pti_max(0, _pti_min(EN_ROOM_HEIGHT - kScreenHeight, cam_y));
	pti_camera(cx, cy);

	pti_map(0, 0);
	RenderAllEntities();

	pti_rectf(cx, cy, kScreenWidth, 16, 0xff000000);

	for (auto i = 0; i < gameState.player->GetHealth(); i++) {
		int x = (cam_x + 8) + (i * 8);
		int y = cam_y + 8;
		pti_spr(bitmap_heart, 0, x - 4, y - 3, false, false);
	}

	// debugging:
	// for (auto *e : GetEntitiesOfType<Actor>()) {
	// 	pti_circ(e->position.x, e->position.y, 16, 0xffff0000);
	// 	pti_rectf(e->position.x + e->bx, e->position.y + e->by, e->bw - 1, e->bh - 1, 0xff00ff00);
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