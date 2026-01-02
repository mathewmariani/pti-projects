#include "pti/pti.h"
#include "bank.h"
#include "gamestate.h"

#include "batteries/actor.h"
#include "batteries/solid.h"

#include <memory>
#include <vector>

#include "batteries/assets.h"
#include "batteries/helper.h"

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

static auto _gameState = std::make_unique<GameState>();

static IWorld *gWorld = nullptr;

IWorld *&World() {
	return gWorld;
}

GameState &GetGameState() {
	return *_gameState;
}

void GameStateInit() {
	World() = &GetGameState();
	((GameState *) World())->Entities.Clear();
}

void GameStateReset() {
	// World()->Entities.Clear();
	_gameState->PlayerIsDead = false;
	_gameState->ResetTimer = 0.0f;
}

template<typename T>
void UpdateEntitiesOfType() {
	GetGameState().Entities.template ForEach<T>([](T *e) {
		e->timer += PTI_DELTA;
		e->Update();
		e->Physics();
	});
}

void GameStateTick() {
	UpdateEntitiesOfType<Solid>();
	UpdateEntitiesOfType<Actor>();
}

void RenderAllEntities() {
	GetGameState().Entities.ForEach<EntityBase>([](EntityBase *e) {
		e->Render();
	});
}

void RemoveEntity(EntityBase *entity) {
	if (entity) {
		GetGameState().Entities.RemoveAt(entity->id);
	}
}

void ChangeLevels(void) {
	// we reload the assets because we alter the RAM when we load level.
	batteries::reload();

	GetGameState().Entities.Clear();
	GetGameState().PlayerIsDead = false;
	GetGameState().ResetTimer = 0.0f;

	auto &levels = GetGameState().levels;
	auto next = -1;
	do {
		next = RandomRange(0, levels.size() - 1);
	} while (next == GetGameState().CurrentLevelIndex);

	pti_set_tilemap(levels[next]);

	int i, j, t;
	for (i = 0; i < EN_ROOM_COLS; i++) {
		for (j = 0; j < EN_ROOM_ROWS; j++) {
			t = pti_mget(i, j);
			switch (t) {
				case 48: {
					if (auto *e = CreateEntity<Player>(); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
				} break;
				case 49:
					Coin::Create({XPOS(i), YPOS(j)});
					pti_mset(i, j, 0);
					break;
				case 50: {
					Goomba::Create({XPOS(i), YPOS(j)});
					pti_mset(i, j, 0);
				} break;
				case 51:
					if (auto *e = CreateEntity<Platform>(Platform::Type::Vertical); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
				case 52:
					if (auto *e = CreateEntity<Platform>(Platform::Type::Horizontal); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
					// shooter
				case 57:
					if (auto *e = CreateEntity<Shooter>(CoordXY<int>::Down); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
				case 58:
					if (auto *e = CreateEntity<Shooter>(CoordXY<int>::Up); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
				case 59:
					if (auto *e = CreateEntity<Shooter>(CoordXY<int>::Right); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
				case 60:
					if (auto *e = CreateEntity<Shooter>(CoordXY<int>::Left); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;

				case 61:
					if (auto *e = CreateEntity<Flag>(); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
			}
		}
	}
}