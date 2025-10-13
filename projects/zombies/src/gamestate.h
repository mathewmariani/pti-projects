#pragma once

#include "entity/registry.h"

#include "entity/actor/bullet.h"
#include "entity/actor/player.h"
#include "entity/actor/zombie.h"

#include <variant>
#include <vector>

constexpr int EN_TILE_SIZE = 8;
constexpr int EN_ROOM_WIDTH = 176;
constexpr int EN_ROOM_HEIGHT = 128;
constexpr int EN_ROOM_COLS = EN_ROOM_WIDTH / EN_TILE_SIZE;
constexpr int EN_ROOM_ROWS = EN_ROOM_HEIGHT / EN_TILE_SIZE;

constexpr float kDeathResetTimer = 2.0f;

using EntityVariant = std::variant<std::monostate, Bullet, Player, Zombie>;

struct GameState_t {
	EntityVariant Entities[kMaxEntities]{};
	uint8_t Coins = 0;
	uint8_t Deaths = 0;

	bool PlayerIsDead = false;
	float ResetTimer = 0.0f;
};

GameState_t &GetGameState();

void GameStateInit();
void GameStateReset();
void GameStateTick();

template<typename T>
std::vector<T *> GetEntitiesOfType() {
	const auto &entities = GetGameState().Entities;
	std::vector<T *> result;
	result.reserve(kMaxEntities);

	for (auto &e : GetGameState().Entities) {
		if (std::holds_alternative<std::monostate>(e)) {
			continue;
		}
		// clang-format off
		std::visit([&](auto& obj) {
			using U = std::decay_t<decltype(obj)>;
			if constexpr (std::is_base_of_v<T, U>) {
				result.push_back(&obj);
			}
		}, e);
		// clang-format on
	}

	return result;
}