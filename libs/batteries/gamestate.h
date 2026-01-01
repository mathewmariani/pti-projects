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

IWorld *&World();