#include "pti/pti.h"
#include "bank.h"
#include "gamestate.h"

#include "entity/actor.h"
#include "entity/solid.h"

#include <memory>
#include <vector>

#include "batteries/assets.h"
#include "batteries/helper.h"

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

static auto _gameState = std::make_unique<GameState_t>();

GameState_t &GetGameState() {
	return *_gameState;
}

void GameStateInit() {
	_gameState->Entities.Clear();
}

void GameStateReset() {
	_gameState->Entities.Clear();
	_gameState->PlayerIsDead = false;
	_gameState->ResetTimer = 0.0f;
}

template<typename T>
void UpdateEntitiesOfType() {
	_gameState->Entities.ForEach<T>([](T *e) {
		e->timer += PTI_DELTA;
		e->Update();
		e->Physics();
	});
}

void GameStateTick() {
	UpdateEntitiesOfType<Solid>();
	UpdateEntitiesOfType<Actor>();
}

void RenderAllEntities() {
	_gameState->Entities.ForEach<EntityBase>([](EntityBase *e) {
		e->Render();
	});
}

void RemoveEntity(EntityBase *entity) {
	if (entity) {
		_gameState->Entities.RemoveAt(entity->id);
	}
}

void ChangeLevels(void) {
	// we reload the assets because we alter the RAM when we load level.
	batteries::reload();

	_gameState->Entities.Clear();
	_gameState->PlayerIsDead = false;
	_gameState->ResetTimer = 0.0f;

	auto &levels = _gameState->levels;
	auto next = -1;
	do {
		next = RandomRange(0, levels.size() - 1);
	} while (next == _gameState->CurrentLevelIndex);

	pti_set_tilemap(levels[next]);

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
			}
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
	const int world_right = EN_ROOM_WIDTH / kTileSize - 1;
	const int world_bottom = EN_ROOM_HEIGHT / kTileSize - 1;

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
			if (pti_fget(pt_tile.x, pt_tile.y) == 0) {
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
	const int world_right = EN_ROOM_WIDTH / kTileSize - 1;
	const int world_bottom = EN_ROOM_HEIGHT / kTileSize - 1;

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