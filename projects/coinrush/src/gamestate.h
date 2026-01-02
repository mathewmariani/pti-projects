#pragma once

#include <unordered_map>
#include <variant>
#include <vector>

// actors
#include "entity/actor/bullet.h"
#include "entity/actor/coin.h"
#include "entity/actor/effect.h"
#include "entity/actor/flag.h"
#include "entity/actor/goomba.h"
#include "entity/actor/player.h"

// solids
#include "entity/solid/platform.h"
#include "entity/solid/shooter.h"

// scenes
#include "scene/game.h"

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
	Game = 0,
};

using CoinRushGameState = batteries::GameState<Bullet, Coin, Effect, Flag, Goomba, Player, Shooter, Platform>;

struct GameState final : public CoinRushGameState {
	void SwitchScenes(SceneType type);

	uint8_t Coins = 0;
	uint8_t Deaths = 0;
	int CurrentLevelIndex = -1;

	std::vector<pti_tilemap_t *> levels;

	bool PlayerIsDead = false;
	float ResetTimer = 0.0f;

private:
	GameScene gameScene;
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