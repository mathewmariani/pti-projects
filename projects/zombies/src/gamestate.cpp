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

struct Rect {
	int x0, y0, x1, y1;
};

std::random_device rd;
std::mt19937 rng(rd());

CoordXY<int> RandomOutsideCamera() {
	std::vector<Rect> candidates;

	int camera_x, camera_y;
	pti_get_camera(&camera_x, &camera_y);

	// Left of camera
	if (camera_x > 0)
		candidates.push_back({0, 0, camera_x - 1, 0 + EN_ROOM_HEIGHT - 1});

	// Right of camera
	if (camera_x + kScreenWidth < 0 + EN_ROOM_WIDTH)
		candidates.push_back({camera_x + kScreenWidth, 0, 0 + EN_ROOM_HEIGHT - 1, 0 + EN_ROOM_HEIGHT - 1});

	// Top of camera
	int overlap_x0 = std::max(0, camera_x);
	int overlap_x1 = std::min(0 + EN_ROOM_HEIGHT - 1, camera_x + kScreenWidth - 1);
	if (camera_y > 0 && overlap_x1 >= overlap_x0)
		candidates.push_back({overlap_x0, 0, overlap_x1, camera_y - 1});

	// Bottom of camera
	if (camera_y + kScreenHeight < 0 + EN_ROOM_HEIGHT && overlap_x1 >= overlap_x0)
		candidates.push_back({overlap_x0, camera_y + kScreenHeight, overlap_x1, 0 + EN_ROOM_HEIGHT - 1});

	if (candidates.empty())
		return {0, 0};// fallback

	// Pick random candidate rectangle
	std::uniform_int_distribution<size_t> rect_dist(0, candidates.size() - 1);
	Rect r = candidates[rect_dist(rng)];

	// Pick random x and y in that rectangle
	std::uniform_int_distribution<int> x_dist(r.x0, r.x1);
	std::uniform_int_distribution<int> y_dist(r.y0, r.y1);

	return {x_dist(rng), y_dist(rng)};
}
