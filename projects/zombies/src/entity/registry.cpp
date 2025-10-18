#include "registry.h"
#include "../gamestate.h"

#include <algorithm>
#include <variant>
#include <vector>

static std::vector<uint8_t> _freeIdList;
static std::vector<EntityBase *> _activeList;

#include "actor/bullet.h"
#include "actor/effect.h"
#include "actor/player.h"
#include "actor/zombie.h"

template<typename T, typename... Args>
EntityBase *CreateEntity(Args &&...args) {
	auto &gameState = GetGameState();
	if (_freeIdList.empty()) return nullptr;
	auto idx = _freeIdList.back();
	_freeIdList.pop_back();

	auto *entity = &gameState.Entities[idx].emplace<T>(std::forward<Args>(args)...);
	entity->id = idx;
	entity->position = {0, 0};
	entity->speed = {0, 0};
	entity->timer = 0.0f;

	_activeList.push_back(entity);
	return entity;
}

// Explicit instantiations (optional)
template EntityBase *CreateEntity<Bullet>();
template EntityBase *CreateEntity<Coin>();
template EntityBase *CreateEntity<Player>();
template EntityBase *CreateEntity<Zombie>();
template EntityBase *CreateEntity<Effect>();

void RemoveEntity(EntityBase *entity) {
	auto &gameState = GetGameState();
	auto index = entity->id;

	// clang-format off
	auto *base = std::visit([](auto &obj) -> EntityBase* {
		using T = std::decay_t<decltype(obj)>;
		if constexpr (std::is_same_v<T, std::monostate>)
		{
			return nullptr;
		}
		else return &obj;
	}, gameState.Entities[index]);
	// clang-format on

	gameState.Entities[index].emplace<std::monostate>();

	_freeIdList.insert(std::upper_bound(std::rbegin(_freeIdList), std::rend(_freeIdList), index).base(), index);
	_activeList.erase(std::remove(_activeList.begin(), _activeList.end(), base), _activeList.end());
}

void ResetAllEntities() {
	auto &gameState = GetGameState();
	for (auto &variant : gameState.Entities) {
		variant.emplace<std::monostate>();
	}

	_activeList.clear();
	_freeIdList.resize(kMaxEntities);

	auto nextId = 0;
	std::for_each(std::rbegin(_freeIdList), std::rend(_freeIdList), [&](auto &elem) {
		elem = nextId;
		nextId++;
	});
}

bool CheckCollisionsWith(const EntityBase *self, EntityBase *&out, const CoordXY<int> &dir) {
	for (auto *other : _activeList) {
		if (self->Overlaps(other, dir)) {
			out = other;
			return true;
		}
	}

	return false;
}

void RenderAllEntities() {
	for (auto *entity : _activeList) {
		entity->Render();
	}
}