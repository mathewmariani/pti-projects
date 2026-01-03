#include "pti/pti.h"

#include <memory>
#include <vector>

// batteries
#include "batteries/assets.h"
#include "batteries/helper.h"

// actors
#include "../entity/actor/player.h"

#include "../bank.h"
#include "../gamestate.h"

#include "game.h"

typedef struct pti_tilemap_t pti_tilemap_t;

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

void GameScene::Init(void) {
	Reset();
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

void GameScene::Update(void) {
	UpdateEntitiesOfType<Solid>();
	UpdateEntitiesOfType<Actor>();

	for (auto *zombie : GetEntitiesOfType<Zombie>()) {
		if (IsWithinSpawnZone(zombie->position)) {
			continue;
		}

		zombie->position = RandomOutsideCamera();
	}
}

void GameScene::Render(void) {
	pti_cls(0xff575757);

	// keep camera inbounds
	int cam_x, cam_y;
	pti_get_camera(&cam_x, &cam_y);
	int cx = _pti_max(0, _pti_min(EN_ROOM_WIDTH - kScreenWidth, cam_x));
	int cy = _pti_max(0, _pti_min(EN_ROOM_HEIGHT - kScreenHeight, cam_y));
	pti_camera(cx, cy);

	pti_map(0, 0);
	RenderEntitiesOfType<EntityBase>();

	pti_rectf(cx, cy, kScreenWidth, 16, 0xff000000);

	for (auto i = 0; i < GetGameState().player->GetHealth(); i++) {
		int x = (cam_x + 8) + (i * 8);
		int y = cam_y + 8;
		pti_spr(bitmap_heart, 0, x - 4, y - 3, false, false);
	}
}