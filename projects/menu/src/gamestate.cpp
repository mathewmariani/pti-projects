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
	_gameState->SwitchScenes(SceneType::Menu);
}
