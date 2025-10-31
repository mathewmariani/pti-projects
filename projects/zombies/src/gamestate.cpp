#include "gamestate.h"

#include "entity/actor.h"
#include "entity/solid.h"

#include <memory>

static auto _gameState = std::make_unique<GameState_t>();

GameState_t &GetGameState() {
	return *_gameState;
}

void GameStateInit() {
	ResetAllEntities();
}

void GameStateReset() {
	ResetAllEntities();
	_gameState->PlayerIsDead = false;
	_gameState->ResetTimer = 0.0f;
}

template<typename T>
void UpdateEntitiesOfType() {
	for (auto &e : GetGameState().Entities) {
		// clang-format off
		std::visit([&](auto &obj) {
			using U = std::decay_t<decltype(obj)>;
			if constexpr (std::is_base_of_v<T, U>) {
				obj.timer += PTI_DELTA;
				obj.Update();
				obj.Physics();
			}
		}, e);
		// clang-format on
	}
}

void GameStateTick() {
	UpdateEntitiesOfType<Solid>();
	UpdateEntitiesOfType<Actor>();
}

#include "pti/pti.h"
#include <random>
#include <vector>
#include <algorithm>
#include "bank.h"

struct Rect {
	int x;
	int y;
	int w;
	int h;
};

int RandInt(int a, int b) {
	if (a > b) std::swap(a, b);
	static thread_local std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<int> dist(a, b);
	return dist(gen);
}

#include <algorithm>

constexpr int kMinSpawnDist = 2;// in tiles
constexpr int kMaxSpawnDist = 4;// in tiles

static inline int TileFromPixel(int px) { return px / kTileSize; }

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
		Side side = static_cast<Side>(RandInt(0, 3));

		switch (side) {
			case TOP: {
				int maxY = cam_top - kMinSpawnDist;
				int minY = std::max(world_top, cam_top - kMaxSpawnDist);
				if (minY <= maxY) {
					int x = RandInt(world_left, world_right);
					int y = RandInt(minY, maxY);
					pt_tile.x = x;
					pt_tile.y = y;
				}
				break;
			}

			case BOTTOM: {
				int minY = cam_bottom + kMinSpawnDist;
				int maxY = std::min(world_bottom, cam_bottom + kMaxSpawnDist);
				if (minY <= maxY) {
					int x = RandInt(world_left, world_right);
					int y = RandInt(minY, maxY);
					pt_tile.x = x;
					pt_tile.y = y;
				}
				break;
			}

			case LEFT: {
				int maxX = cam_left - kMinSpawnDist;
				int minX = std::max(world_left, cam_left - kMaxSpawnDist);
				if (minX <= maxX) {
					int x = RandInt(minX, maxX);
					int y = RandInt(world_top, world_bottom);
					pt_tile.x = x;
					pt_tile.y = y;
				}
				break;
			}

			case RIGHT: {
				int minX = cam_right + kMinSpawnDist;
				int maxX = std::min(world_right, cam_right + kMaxSpawnDist);
				if (minX <= maxX) {
					int x = RandInt(minX, maxX);
					int y = RandInt(world_top, world_bottom);
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
