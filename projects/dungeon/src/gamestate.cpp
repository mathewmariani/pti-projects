#include "pti/pti.h"
#include "bank.h"
#include "gamestate.h"

#include "batteries/actor.h"

#include <memory>
#include <vector>

#include "batteries/assets.h"
#include "batteries/helper.h"

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

void GameState::SwitchScenes(SceneType type) {
	switch (type) {
		case SceneType::Game: {
			CurrentScene = &gameScene;
		} break;
	}

	ChangeLevels();
}

static auto _gameState = std::make_unique<GameState>();

batteries::IScene *Scene() {
	return (batteries::IScene *) _gameState->CurrentScene;
}

GameState &GetGameState() {
	return *_gameState;
}

void GameStateInit() {
	_gameState->SwitchScenes(SceneType::Game);

	// reset the game state
	_gameState->PlayerIsDead = false;
	_gameState->ResetTimer = 0.0f;
}

void ChangeLevels() {
	// we reload the assets because we alter the RAM when we load level.
	batteries::reload();

	auto &levels = GetGameState().levels;
	auto next = -1;
	do {
		next = RandomRange(0, levels.size() - 1);
	} while (next == GetGameState().CurrentLevelIndex);

	pti_set_tilemap(levels[next]);

	_gameState->CurrentScene->Init();
}