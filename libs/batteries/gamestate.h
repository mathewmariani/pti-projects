#pragma once

#include <variant>
#include <vector>

#include "registry.h"
#include "world.h"
#include "scene.h"

#include "solid.h"
#include "actor.h"

constexpr int kMaxEntities = 256;

template<typename... EntityTypes>
struct GameWorld : IWorld {
	batteries::Scene<kMaxEntities, EntityTypes...> *CurrentScene;

	void ForEachActor(const std::function<void(Actor &)> &fn) override {
		for (auto *actor : CurrentScene->template GetEntitiesOfType<Actor>()) {
			if (actor) {
				fn(*actor);
			}
		}
	}

	void ForEachSolid(const std::function<void(Solid &)> &fn) override {
		for (auto *solid : CurrentScene->template GetEntitiesOfType<Solid>()) {
			if (solid) {
				fn(*solid);
			}
		}
	}
};

IWorld *&World();