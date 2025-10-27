/* engine */
#include "pti/pti.h"

#include "bank.h"
#include "gamestate.h"
#include "batteries/assets.h"
#include "batteries/registry.h"

#include <string>

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

static void load(void) {
	GameStateInit();
	batteries::reload();

	int i, j, t;
	for (i = 0; i < EN_ROOM_COLS; i++) {
		for (j = 0; j < EN_ROOM_ROWS; j++) {
			t = pti_mget(tilemap, i, j);
			switch (t) {
				case 48: {
					if (auto *e = CreateEntity<Player>(); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(tilemap, i, j, 0);
						GetGameState().player = static_cast<Player *>(e);
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
	bitmap_player = batteries::sprite("assets/dog.ase");
	bitmap_zombie = batteries::sprite("assets/zombie.ase");
	bitmap_heart = batteries::sprite("assets/heart.ase");
	bitmap_platform = batteries::sprite("assets/platform.ase");
	bitmap_font = batteries::sprite("assets/font.ase");
	bitmap_fx_collect = batteries::sprite("assets/collect.ase");

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

	pti_cls(0xffef7d57);

	// keep camera inbounds
	int cam_x, cam_y;
	pti_get_camera(&cam_x, &cam_y);
	int cx = _pti_max(0, _pti_min(EN_ROOM_WIDTH - kScreenWidth, cam_x));
	int cy = _pti_max(0, _pti_min(EN_ROOM_HEIGHT - kScreenHeight, cam_y));
	pti_camera(cx, cy);

	pti_map(tilemap, tileset, 0, 0);
	RenderAllEntities();

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