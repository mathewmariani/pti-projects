#pragma once

#include <variant>
#include <vector>

#include "registry.h"
#include "world.h"

#include "solid.h"
#include "actor.h"

constexpr int kMaxEntities = 256;

template<typename... EntityTypes>
struct GameWorld : IWorld {
	EntityManager<kMaxEntities, EntityTypes...> Entities;

	void ForEachActor(const std::function<void(Actor &)> &fn) override {
		Entities.template ForEach<Actor>([&](Actor *a) {
			fn(*a);
		});
	}

	void ForEachSolid(const std::function<void(Solid &)> &fn) override {
		Entities.template ForEach<Solid>([&](Solid *s) {
			fn(*s);
		});
	}
};

template<typename... EntityTypes>
struct GameState_t {
	EntityManager<kMaxEntities, EntityTypes...> Entities;
};

IWorld *&World();

namespace batteries {

	template<typename T, typename EntityManagerT, typename... Args>
	T *CreateEntity(EntityManagerT &entities, Args &&...args) {
		return entities.template Create<T>(std::forward<Args>(args)...);
	}

	template<typename EntityManagerT>
	void RemoveEntity(EntityManagerT &entities, EntityBase *entity) {
		if (!entity) {
			return;
		}
		entities.RemoveAt(entity->id);
	}

	template<typename T, typename EntityManagerT>
	std::vector<T *> GetEntitiesOfType(EntityManagerT &entities) {
		return entities.template GetList<T>();
	}

	template<typename T, typename EntityManagerT>
	std::vector<T *> GetCollisions(EntityManagerT &entities, EntityBase *subject, const CoordXY<int> &dir) {
		std::vector<T *> result;

		entities.template ForEach<T>([&](T *other) {
			if (subject->Overlaps(other, dir)) {
				result.push_back(other);
			}
		});

		return result;
	}

}// namespace batteries