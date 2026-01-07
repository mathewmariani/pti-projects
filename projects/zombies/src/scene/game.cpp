#include "pti/pti.h"

#include <memory>
#include <vector>

// batteries
#include "batteries/actor.h"
#include "batteries/solid.h"
#include "batteries/assets.h"

// actors
#include "../entity/actor/player.h"

#include "game.h"
#include "../bank.h"
#include "../gamestate.h"

// should be defined elsewhere
constexpr int kTileSize = 8;
constexpr float kDeathResetTimer = 2.0f;
constexpr int kScreenWidth = 176;
constexpr int kScreenHeight = 128;
constexpr int kWorldWidth = 528;
constexpr int kWorldHeight = 384;

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

bool flag = false;

void GameScene::Init(void) {
	{ // FIXME: ugly hack.
		if (!flag) {
			batteries::init();
			palette = batteries::palette("assets/palette.hex");
			flags = batteries::flags("assets/flags.bin");
			tileset = batteries::tileset("assets/tilemap.ase");
			tilemap = batteries::tilemap("assets/tilemap.ase");
			bitmap_bullet = batteries::sprite("assets/bullet.ase");
			bitmap_coin = batteries::sprite("assets/coin.ase");
			bitmap_player = batteries::sprite("assets/dog.ase");
			bitmap_zombie = batteries::sprite("assets/zombie.ase");
			bitmap_heart = batteries::sprite("assets/heart.ase");
			bitmap_font = batteries::sprite("assets/font.ase");
			bitmap_fx_collect = batteries::sprite("assets/collect.ase");

			pti_set_palette(palette);
			pti_set_flags(flags);
			pti_set_tilemap(tilemap);
			pti_set_tileset(tileset);
			pti_set_font(bitmap_font);

			flag = true;
		}

		// reload loads the specific bank into pti
		batteries::reload();
	}

	Reset();
	int i, j, t;
	for (i = 0; i < kWorldHeight / kTileSize; i++) {
		for (j = 0; j < kWorldWidth / kTileSize; j++) {
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

#include <algorithm>
#include "batteries/helper.h"

constexpr int kMinSpawnDist = 2;// in tiles
constexpr int kMaxSpawnDist = 4;// in tiles

static inline int TileFromPixel(int px) {
	return px / kTileSize;
}

CoordXY<int> RandomOutsideCamera() {
	int camera_x_px, camera_y_px;
	pti_get_camera(&camera_x_px, &camera_y_px);

	const int world_left = 0;
	const int world_top = 0;
	const int world_right = kWorldWidth / kTileSize - 1;
	const int world_bottom = kWorldHeight / kTileSize - 1;

	const int cam_left = std::max(world_left, TileFromPixel(camera_x_px));
	const int cam_top = std::max(world_top, TileFromPixel(camera_y_px));
	const int cam_right = std::min(world_right, TileFromPixel(camera_x_px + kScreenWidth - 1));
	const int cam_bottom = std::min(world_bottom, TileFromPixel(camera_y_px + kScreenHeight - 1));

	CoordXY<int> pt_tile{-1, -1};

	for (int tries = 0; tries < 100; ++tries) {
		enum Side { TOP,
					BOTTOM,
					LEFT,
					RIGHT };
		Side side = static_cast<Side>(RandomRange(0, 3));

		switch (side) {
			case TOP: {
				int maxY = cam_top - kMinSpawnDist;
				int minY = std::max(world_top, cam_top - kMaxSpawnDist);
				if (minY <= maxY) {
					int x = RandomRange(world_left, world_right);
					int y = RandomRange(minY, maxY);
					pt_tile.x = x;
					pt_tile.y = y;
				}
				break;
			}

			case BOTTOM: {
				int minY = cam_bottom + kMinSpawnDist;
				int maxY = std::min(world_bottom, cam_bottom + kMaxSpawnDist);
				if (minY <= maxY) {
					int x = RandomRange(world_left, world_right);
					int y = RandomRange(minY, maxY);
					pt_tile.x = x;
					pt_tile.y = y;
				}
				break;
			}

			case LEFT: {
				int maxX = cam_left - kMinSpawnDist;
				int minX = std::max(world_left, cam_left - kMaxSpawnDist);
				if (minX <= maxX) {
					int x = RandomRange(minX, maxX);
					int y = RandomRange(world_top, world_bottom);
					pt_tile.x = x;
					pt_tile.y = y;
				}
				break;
			}

			case RIGHT: {
				int minX = cam_right + kMinSpawnDist;
				int maxX = std::min(world_right, cam_right + kMaxSpawnDist);
				if (minX <= maxX) {
					int x = RandomRange(minX, maxX);
					int y = RandomRange(world_top, world_bottom);
					pt_tile.x = x;
					pt_tile.y = y;
				}
				break;
			}
		}

		if (pt_tile.x >= 0 && pt_tile.y >= 0) {
			auto tile = pti_mget(pt_tile.x, pt_tile.y);
			if (pti_fget(tile) == 0) {
				return pt_tile * kTileSize;
			}
		}
	}

	return CoordXY<int>{world_left * kTileSize, world_top * kTileSize};
}

bool IsWithinSpawnZone(const CoordXY<int> &worldPos) {
	int camera_x_px, camera_y_px;
	pti_get_camera(&camera_x_px, &camera_y_px);

	const int world_left = 0;
	const int world_top = 0;
	const int world_right = kWorldWidth / kTileSize - 1;
	const int world_bottom = kWorldHeight / kTileSize - 1;

	const int cam_left = std::max(world_left, camera_x_px / kTileSize);
	const int cam_top = std::max(world_top, camera_y_px / kTileSize);
	const int cam_right = std::min(world_right, (camera_x_px + kScreenWidth - 1) / kTileSize);
	const int cam_bottom = std::min(world_bottom, (camera_y_px + kScreenHeight - 1) / kTileSize);

	CoordXY<int> pt = worldPos / kTileSize;

	if (pt.x < world_left || pt.y < world_top || pt.x > world_right || pt.y > world_bottom) {
		return false;
	}

	int dx = 0;
	if (pt.x < cam_left) {
		dx = cam_left - pt.x;
	} else if (pt.x > cam_right) {
		dx = pt.x - cam_right;
	}

	int dy = 0;
	if (pt.y < cam_top) {
		dy = cam_top - pt.y;
	} else if (pt.y > cam_bottom) {
		dy = pt.y - cam_bottom;
	}

	return std::max(dx, dy) <= kMaxSpawnDist;
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

	for (auto *zombie : GetEntitiesOfType<Zombie>()) {
		if (IsWithinSpawnZone(zombie->position)) {
			continue;
		}

		zombie->position = RandomOutsideCamera();
	}
}

void GameScene::Render(void) {
	pti_cls(15);

	// keep camera inbounds
	int cam_x, cam_y;
	pti_get_camera(&cam_x, &cam_y);
	int cx = _pti_max(0, _pti_min(kWorldWidth - kScreenWidth, cam_x));
	int cy = _pti_max(0, _pti_min(kWorldHeight - kScreenHeight, cam_y));
	pti_camera(cx, cy);

	pti_map(0, 0);
	RenderEntitiesOfType<EntityBase>();

	pti_rectf(cx, cy, kScreenWidth, 16, 1);

	for (auto i = 0; i < GetGameState().player->GetHealth(); i++) {
		int x = (cam_x + 8) + (i * 8);
		int y = cam_y + 8;
		pti_spr(bitmap_heart, 0, x - 4, y - 3, false, false);
	}
}