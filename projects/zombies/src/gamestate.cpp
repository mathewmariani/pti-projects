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

// Simple Rect type
struct Rect {
    int x; // top-left corner
    int y;
    int w;
    int h;
};

// Helper for random integer in [a, b]
int RandInt(int a, int b) {
    if (a > b) std::swap(a, b);
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(a, b);
    return dist(gen);
}

#include <algorithm>

constexpr int kMinSpawnDist = 4; // in tiles

CoordXY<int> RandomOutsideCamera() {
    int camera_x, camera_y;
    pti_get_camera(&camera_x, &camera_y);

    const Rect world{
        .x = 0,
        .y = 0,
        .w = EN_ROOM_WIDTH / EN_TILE_SIZE - 1,
        .h = EN_ROOM_HEIGHT / EN_TILE_SIZE - 1,
    };

    const Rect camera{
        .x = std::max(0, camera_x / EN_TILE_SIZE),
        .y = std::max(0, camera_y / EN_TILE_SIZE),
        .w = std::min(world.w, (camera_x + kScreenWidth) / EN_TILE_SIZE),
        .h = std::min(world.h, (camera_y + kScreenHeight) / EN_TILE_SIZE),
    };

    CoordXY<int> pt{};

    for (int tries = 0; tries < 100; ++tries) {
        enum Side { TOP, BOTTOM, LEFT, RIGHT };
        Side side = static_cast<Side>(RandInt(0, 3));

        switch (side) {
            case TOP:
                if (camera.y - kMinSpawnDist > world.y) {
                    pt.x = RandInt(world.x, world.x + world.w);
                    pt.y = RandInt(world.y, camera.y - kMinSpawnDist);
                }
                break;

            case BOTTOM:
                if (camera.y + camera.h + kMinSpawnDist < world.y + world.h) {
                    pt.x = RandInt(world.x, world.x + world.w);
                    pt.y = RandInt(camera.y + camera.h + kMinSpawnDist, world.y + world.h);
                }
                break;

            case LEFT:
                if (camera.x - kMinSpawnDist > world.x) {
                    pt.x = RandInt(world.x, camera.x - kMinSpawnDist);
                    pt.y = RandInt(world.y, world.y + world.h);
                }
                break;

            case RIGHT:
                if (camera.x + camera.w + kMinSpawnDist < world.x + world.w) {
                    pt.x = RandInt(camera.x + camera.w + kMinSpawnDist, world.x + world.w);
                    pt.y = RandInt(world.y, world.y + world.h);
                }
                break;
        }

        // Validate and return
        if (pti_fget(tilemap, pt.x, pt.y) == 0)
            return pt;
    }

    // Fallback if nothing valid found
    return {world.x, world.y};
}
