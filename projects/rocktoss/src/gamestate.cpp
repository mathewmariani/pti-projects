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

static IWorld *gWorld = nullptr;

IWorld *&World() {
	return gWorld;
}

GameState &GetGameState() {
	return *_gameState;
}

void GameStateInit() {
	_gameState->SwitchScenes(SceneType::Menu);
	World() = &GetGameState();
	((GameState *) World())->Entities.Clear();
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