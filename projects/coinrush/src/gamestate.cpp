#include "pti/pti.h"
#include "bank.h"
#include "gamestate.h"

#include "entity/actor.h"
#include "entity/solid.h"

#include <memory>
#include <vector>

#include "batteries/math.h"

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

static auto _gameState = std::make_unique<GameState_t>();

GameState_t &GetGameState() {
	return *_gameState;
}

void GameStateInit() {
	_gameState->Entities.Clear();
}

void GameStateReset() {
	_gameState->Entities.Clear();
	_gameState->PlayerIsDead = false;
	_gameState->ResetTimer = 0.0f;
}

template<typename T>
void UpdateEntitiesOfType() {
	_gameState->Entities.ForEach<T>([](T *e) {
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
	_gameState->Entities.ForEach<EntityBase>([](EntityBase *e) {
		e->Render();
	});
}

void RemoveEntity(EntityBase *entity) {
	if (entity) {
		_gameState->Entities.RemoveAt(entity->id);
	}
}

void ChangeLevels(void) {
	_gameState->Entities.Clear();

	auto &levels = _gameState->levels;
	pti_set_tilemap(levels[RandomRange(0, levels.size() - 1)]);

	int i, j, t;
	for (i = 0; i < EN_ROOM_COLS; i++) {
		for (j = 0; j < EN_ROOM_ROWS; j++) {
			t = pti_mget(i, j);
			switch (t) {
				case 48: {
					if (auto *e = CreateEntity<Player>(); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
						GetGameState().player = static_cast<Player *>(e);
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