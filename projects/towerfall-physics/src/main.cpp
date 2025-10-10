/* engine */
#include "pti/pti.h"

#include "bank.h"
#include "gamestate.h"
#include "batteries/assets.h"

/* solids */
#include "entity/solid/platform.h"

/* actors */
#include "entity/actor/player.h"

#include "entity/registry.h"

#include <math.h>

#include <string>

namespace {
	constexpr int width = 176;
	constexpr int height = 128;
	auto t = 0.0f;

	float resetTimer = 0.0f;
}// namespace

#define XPOS(x) (x * EN_TILE_SIZE)
#define YPOS(y) (y * EN_TILE_SIZE)

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
					}
				} break;
				case 51:
					if (auto *e = CreateEntity<Platform>(Platform::Type::Vertical); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(tilemap, i, j, 0);
					}
					break;
				case 52:
					if (auto *e = CreateEntity<Platform>(Platform::Type::Horizontal); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(tilemap, i, j, 0);
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
	bitmap_player = batteries::sprite("assets/dog.ase");
	bitmap_platform = batteries::sprite("assets/platform.ase");
	bitmap_font = batteries::sprite("assets/font.ase");

	load();

	/* graphics state: */
	pti_dither(0x5a5a);
	pti_clip(0, 0, EN_ROOM_WIDTH, EN_ROOM_HEIGHT);
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

	/* adjust camera */
	int cam_x, cam_y;
	pti_get_camera(&cam_x, &cam_y);

	int cx = _pti_max(0, _pti_min(EN_ROOM_WIDTH - width, cam_x));
	int cy = _pti_max(0, _pti_min(EN_ROOM_HEIGHT - height, cam_y));
	pti_camera(cx, cy);

	pti_map(tilemap, tileset, 0, 0);
	RenderAllEntities();

	// const auto status_str = std::format("coins: &d\n", coins);
	char buffer[100];
	std::snprintf(buffer, sizeof(buffer), "Is Dead: %s\n", gameState.PlayerIsDead ? "True" : "False");
	std::string status_str(buffer);

	pti_print(bitmap_font, status_str.c_str(), 4, 0);
}

pti_desc pti_main(int argc, char *argv[]) {

	return (pti_desc) {
			.init_cb = init,
			.frame_cb = frame,
			.cleanup_cb = cleanup,
			.memory_size = _pti_kilobytes(256), /* 256KB */
			.width = 176,
			.height = 128,
	};
}