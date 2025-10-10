#include "registry.h"
#include "../gamestate.h"

#include <algorithm>
#include <array>
#include <variant>
#include <vector>
#include <cstdio>

static std::vector<uint8_t> _freeIdList;

namespace {
	auto GetEntityBase = [](auto &entity) -> EntityBase * { return &entity; };
}// namespace

#include "actor/player.h"
#include "solid/platform.h"

template<typename T, typename... Args>
EntityBase *CreateEntity(Args &&...args) {
	auto &gameState = GetGameState();
	if (_freeIdList.empty()) return nullptr;
	auto idx = _freeIdList.back();
	_freeIdList.pop_back();

	EntityBase *entity = nullptr;

	if constexpr (std::is_same_v<T, Player>)
		entity = reinterpret_cast<EntityBase *>(&gameState.Entities[idx].emplace<Player>(std::forward<Args>(args)...));
	else if constexpr (std::is_same_v<T, Platform>)
		entity = reinterpret_cast<EntityBase *>(&gameState.Entities[idx].emplace<Platform>(std::forward<Args>(args)...));
	else
		return nullptr;

	entity->type = T::cEntityType;
	entity->id = idx;
	entity->x = 0;
	entity->y = 0;
	return entity;
}

// Explicit instantiations (optional)
template EntityBase *CreateEntity<Player>();
template EntityBase *CreateEntity<Platform, Platform::Type>(Platform::Type &&);

void RemoveEntity(EntityBase *entity) {
	// add to free list
	auto index = entity->id;
	_freeIdList.insert(std::upper_bound(std::rbegin(_freeIdList), std::rend(_freeIdList), index).base(), index);

	// reset entity
	*entity = EntityBase();
	entity->id = index;
	entity->type = EntityType::Null;
}

void ResetAllEntities() {
	auto &gameState = GetGameState();
	std::fill(std::begin(gameState.Entities), std::end(gameState.Entities), EntityBase());

	_freeIdList.clear();
	_freeIdList.resize(kMaxEntities);

	auto nextId = 0;
	std::for_each(std::rbegin(_freeIdList), std::rend(_freeIdList), [&](auto &elem) {
		elem = nextId;
		nextId++;
	});
}

bool CheckCollisionsWith(const EntityBase *self, EntityBase *&out, const CoordXY<int> &dir) {
	for (auto &a : GetGameState().Entities) {
		if (!self || self->type == EntityType::Null) {
			continue;
		}

		auto *other = std::visit(GetEntityBase, a);
		if (!other || other->type == EntityType::Null || self == other) {
			continue;
		}

		if (self->Overlaps(other, dir)) {
			out = other;
			return true;
		}
	}

	return false;
}

void RenderAllEntities() {
	for (auto &e : GetGameState().Entities) {
		auto *entity = std::visit(GetEntityBase, e);
		if (entity && entity->type != EntityType::Null) {
			entity->Render();
		}
	}
}