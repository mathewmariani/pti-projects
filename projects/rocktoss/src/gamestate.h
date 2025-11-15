#pragma once

#include <unordered_map>
#include <variant>
#include <vector>

// actors
#include "entity/actor/player.h"

// scenes
#include "scene/game.h"
#include "scene/menu.h"

// batteries
#include "batteries/registry.h"
#include "batteries/scene.h"

typedef struct pti_tilemap_t pti_tilemap_t;

constexpr int kScreenWidth = 176;
constexpr int kScreenHeight = 128;
constexpr int kTileSize = 8;
constexpr int EN_ROOM_WIDTH = (kScreenWidth);
constexpr int EN_ROOM_HEIGHT = (kScreenHeight);
constexpr int EN_ROOM_COLS = EN_ROOM_WIDTH / kTileSize;
constexpr int EN_ROOM_ROWS = EN_ROOM_HEIGHT / kTileSize;
#define PTI_DELTA (1.0 / 30.0)
constexpr float kDeathResetTimer = 2.0f;

using BasicScene = batteries::Scene<256, Player>;

enum class SceneType {
	Menu = 0,
	Game = 1,
};

struct GameState {
	BasicScene *CurrentScene;
	void SwitchScenes(SceneType type);

	// game specific variables
	int Coins = 0;

private:
	GameScene gameScene;
	MenuScene menuScene;
};

GameState &GetGameState();

void GameStateInit();
void GameStateReset();
void GameStateTick();

void RenderAllEntities();

template<typename T, typename... Args>
inline EntityBase *CreateEntity(Args &&...args) {
	return GetGameState().CurrentScene->CreateEntity<T>(std::forward<Args>(args)...);
}

void RemoveEntity(EntityBase *entity);

template<typename T>
inline std::vector<T *> GetEntitiesOfType() {
	return GetGameState().CurrentScene->GetEntitiesOfType<T>();
}

template<typename T>
inline std::vector<T *> GetCollisions(EntityBase *subject, const CoordXY<int> &dir) {
	return GetGameState().CurrentScene->GetCollisions<T>(subject, dir);
}
