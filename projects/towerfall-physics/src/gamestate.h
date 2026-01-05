#pragma once

#include <vector>

// scenes
#include "scene/game.h"

// batteries
#include "batteries/gamestate.h"
#include "batteries/registry.h"
#include "batteries/scene.h"

#define PTI_DELTA (1 / 30.0f)

enum class SceneType {
	Game = 0,
};

struct GameState final : public batteries::GameState {
	void SwitchScenes(SceneType type);

	bool PlayerIsDead = false;
	float ResetTimer = 0.0f;

private:
	GameScene gameScene;
};

GameState &GetGameState();

void GameStateInit();

// helpers:

template<typename T, typename... Args>
inline EntityBase *CreateEntity(Args &&...args) {
	return ((BasicScene *) Scene())->CreateEntity<T>(std::forward<Args>(args)...);
}

template<typename T>
inline std::vector<T *> GetEntitiesOfType() {
	return ((BasicScene *) Scene())->GetEntitiesOfType<T>();
}

template<typename T>
inline std::vector<T *> GetCollisions(EntityBase *subject, const CoordXY<int> &dir) {
	return ((BasicScene *) Scene())->GetCollisions<T>(subject, dir);
}