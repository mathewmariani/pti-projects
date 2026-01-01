#include "pti/pti.h"
#include "bank.h"
#include "gamestate.h"

#include <memory>
#include <vector>

#include "batteries/assets.h"
#include "batteries/physics.h"
#include "batteries/helper.h"

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

static auto _gameState = std::make_unique<GameState>();

GameState &GetGameState() {
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
	PhysicsSystem physics{GetGameState().Entities};
	_gameState->Entities.ForEach<Solid>([&](Solid *solid) {
		solid->timer += PTI_DELTA;
		solid->Update();
		physics.SolidMoveX(*solid, solid->speed.x);
		physics.SolidMoveY(*solid, solid->speed.y);
	});

	_gameState->Entities.ForEach<Actor>([&](Actor *actor) {
		actor->timer += PTI_DELTA;
		actor->Update();
		physics.ActorMoveX(*actor, actor->speed.x, &Actor::HaltX);
		physics.ActorMoveY(*actor, actor->speed.y, &Actor::HaltY);

		actor->grounded = physics.IsActorGrounded(*actor);
	});
}

void RenderAllEntities() {
	_gameState->Entities.ForEach<EntityBase>([](EntityBase *e) {
		e->Render();
	});
}

void ChangeLevels(void) {
	// we reload the assets because we alter the RAM when we load level.
	batteries::reload();

	_gameState->Entities.Clear();
	_gameState->PlayerIsDead = false;
	_gameState->ResetTimer = 0.0f;

	auto &levels = _gameState->levels;
	auto next = -1;
	do {
		next = RandomRange(0, levels.size() - 1);
	} while (next == _gameState->CurrentLevelIndex);

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
				case 51: {
					if (auto *e = CreateEntity<Platform>(Platform::Type::Vertical); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
				} break;
				case 52: {
					if (auto *e = CreateEntity<Platform>(Platform::Type::Horizontal); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
				} break;
			}
		}
	}
}