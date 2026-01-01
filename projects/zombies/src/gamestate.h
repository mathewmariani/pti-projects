#pragma once

#include <variant>
#include <vector>

#include "batteries/gamestate.h"
#include "batteries/registry.h"

// actors
#include "entity/actor/bullet.h"
#include "entity/actor/coin.h"
#include "entity/actor/effect.h"
#include "entity/actor/player.h"
#include "entity/actor/zombie.h"

typedef struct pti_tilemap_t pti_tilemap_t;

constexpr int kScreenWidth = 176;
constexpr int kScreenHeight = 128;
constexpr int kWorldWidth = 528;
constexpr int kWorldHeight = 384;
constexpr int kTileSize = 8;
constexpr int EN_ROOM_WIDTH = (kWorldWidth);
constexpr int EN_ROOM_HEIGHT = (kWorldHeight);
constexpr int EN_ROOM_COLS = EN_ROOM_WIDTH / kTileSize;
constexpr int EN_ROOM_ROWS = EN_ROOM_HEIGHT / kTileSize;
#define PTI_DELTA (1.0 / 30.0)
constexpr float kDeathResetTimer = 2.0f;

using ThisNeedsAName = GameWorld<Bullet, Coin, Effect, Player, Zombie>;

struct GameState final : public ThisNeedsAName {
	uint8_t Coins = 0;
	uint8_t Deaths = 0;
	int CurrentLevelIndex = -1;

	std::vector<pti_tilemap_t *> levels;
	Player *player;

	bool PlayerIsDead = false;
	float ResetTimer = 0.0f;
};

GameState &GetGameState();

void GameStateInit();
void GameStateReset();
void GameStateTick();

void ChangeLevels();

void RenderAllEntities();

template<typename T, typename... Args>
T *CreateEntity(Args &&...args) {
	return batteries::CreateEntity<T>(GetGameState().Entities, std::forward<Args>(args)...);
}

inline void RemoveEntity(EntityBase *entity) {
	batteries::RemoveEntity(GetGameState().Entities, entity);
}

template<typename T>
std::vector<T *> GetEntitiesOfType() {
	return batteries::GetEntitiesOfType<T>(GetGameState().Entities);
}

template<typename T>
std::vector<T *> GetCollisions(EntityBase *subject, const CoordXY<int> &dir) {
	return batteries::GetCollisions<T>(GetGameState().Entities, subject, dir);
}

CoordXY<int> RandomOutsideCamera();
bool IsWithinSpawnZone(const CoordXY<int> &pt);