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
