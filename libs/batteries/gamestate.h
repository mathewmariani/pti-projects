#pragma once

#include <variant>
#include <vector>

#include "registry.h"

constexpr int kMaxEntities = 256;

template<typename... EntityTypes>
struct GameState_t {
	EntityManager<kMaxEntities, EntityTypes...> Entities;
};

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