#include "gamestate.h"

#include "batteries/assets.h"
#include "batteries/helper.h"

#include <memory>

void GameState::SwitchScenes(SceneType type) {
	switch (type) {
		case SceneType::Game: {
			CurrentScene = &gameScene;
		} break;
	}

	CurrentScene->Init();
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