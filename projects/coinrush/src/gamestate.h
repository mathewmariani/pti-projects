#pragma once

#include <variant>
#include <vector>

#include "batteries/registry.h"

// actors
#include "entity/actor/bullet.h"
#include "entity/actor/coin.h"
#include "entity/actor/effect.h"
#include "entity/actor/flag.h"
#include "entity/actor/goomba.h"
#include "entity/actor/player.h"
#include "entity/actor/shooter.h"

// solids
#include "entity/solid/platform.h"

constexpr int kMaxEntities = 256;

constexpr int kScreenWidth = 320;
constexpr int kScreenHeight = 224;
constexpr int kTileSize = 8;
constexpr int EN_ROOM_WIDTH = (kScreenWidth);
constexpr int EN_ROOM_HEIGHT = (kScreenHeight);
constexpr int EN_ROOM_COLS = EN_ROOM_WIDTH / kTileSize;
constexpr int EN_ROOM_ROWS = EN_ROOM_HEIGHT / kTileSize;
#define PTI_DELTA (1.0 / 30.0)
constexpr float kDeathResetTimer = 2.0f;

struct GameState_t {
	EntityManager<kMaxEntities, Bullet, Coin, Effect, Flag, Goomba, Player, Shooter, Platform> Entities;
	uint8_t Coins = 0;
	uint8_t Deaths = 0;

	Player *player;

	bool PlayerIsDead = false;
	float ResetTimer = 0.0f;
};

GameState_t &GetGameState();

void GameStateInit();
void GameStateReset();
void GameStateTick();

void RenderAllEntities();

template<typename T, typename... Args>
EntityBase *CreateEntity(Args &&...args) {
	return GetGameState().Entities.Create<T>(std::forward<Args>(args)...);
}
void RemoveEntity(EntityBase *entity);

CoordXY<int> RandomOutsideCamera();
bool IsWithinSpawnZone(const CoordXY<int> &pt);

template<typename T>
std::vector<T *> GetEntitiesOfType() {
	return GetGameState().Entities.GetList<T>();
}

template<typename T>
std::vector<T *> GetCollisions(EntityBase *subject, const CoordXY<int> &dir) {
	std::vector<T *> result;
	result.reserve(kMaxEntities);

	GetGameState().Entities.ForEach<T>([&](T *other) {
		if (subject->Overlaps(other, dir)) {
			result.push_back(other);
		}
	});
	return result;
}
