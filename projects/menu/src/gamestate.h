#pragma once

#include <vector>

// scenes
#include "scene/menu.h"

// batteries
#include "batteries/gamestate.h"
#include "batteries/registry.h"
#include "batteries/scene.h"

typedef struct pti_tilemap_t pti_tilemap_t;

constexpr int kScreenWidth = 320;
constexpr int kScreenHeight = 224;
constexpr int kTileSize = 8;
constexpr int EN_ROOM_WIDTH = (kScreenWidth);
constexpr int EN_ROOM_HEIGHT = (kScreenHeight);
constexpr int EN_ROOM_COLS = EN_ROOM_WIDTH / kTileSize;
constexpr int EN_ROOM_ROWS = EN_ROOM_HEIGHT / kTileSize;
#define PTI_DELTA (1.0 / 30.0)
constexpr float kDeathResetTimer = 2.0f;

enum class SceneType {
	Menu = 0,
};

struct GameState final : public batteries::GameState {
	void SwitchScenes(SceneType type);

private:
	MenuScene menuScene;
};

GameState &GetGameState();

void GameStateInit();
void ChangeLevels();

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