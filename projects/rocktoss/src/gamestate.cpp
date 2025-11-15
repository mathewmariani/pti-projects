#include "pti/pti.h"
#include "bank.h"
#include "gamestate.h"

#include "entity/actor.h"

#include <memory>
#include <vector>

#include "batteries/assets.h"
#include "batteries/helper.h"

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

void GameState::SwitchScenes(SceneType type) {
	switch (type) {
		case SceneType::Menu: {
			CurrentScene = &menuScene;
		} break;
		case SceneType::Game: {
			CurrentScene = &gameScene;
		} break;
	}

	CurrentScene->Init();
}

static auto _gameState = std::make_unique<GameState>();

GameState &GetGameState() {
	return *_gameState;
}

void GameStateInit() {
	_gameState->SwitchScenes(SceneType::Menu);
}

void GameStateReset() {
	_gameState->CurrentScene->Reset();
}

void GameStateTick() {
	_gameState->CurrentScene->Update();
	_gameState->CurrentScene->Render();
}

void RemoveEntity(EntityBase *entity) {
	_gameState->CurrentScene->RemoveEntity(entity);
}