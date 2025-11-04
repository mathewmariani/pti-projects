#include "pti/pti.h"
#include "bank.h"
#include "gamestate.h"

#include "entity/actor.h"
#include "entity/solid.h"

#include <memory>
#include <vector>

#include "batteries/math.h"

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

// ---------------------------------------------------
// Update entities of exact type
template<typename T>
void UpdateEntitiesOfType() {
	_gameState->Entities.ForEach<T>([](T *e) {
		e->timer += PTI_DELTA;
		e->Update();
		e->Physics();
	});
}

// Tick the game
void GameStateTick() {
	UpdateEntitiesOfType<Solid>();
	UpdateEntitiesOfType<Actor>();
}

// Render all entities (polymorphic)
void RenderAllEntities() {
	_gameState->Entities.ForEach<EntityBase>([](EntityBase *e) {
		e->Render();
	});
}

// Remove entity safely
void RemoveEntity(EntityBase *entity) {
	if (entity) {
		_gameState->Entities.RemoveAt(entity->id);
	}
}