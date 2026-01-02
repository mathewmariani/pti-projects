#pragma once

#include <variant>
#include <vector>

#include "registry.h"
#include "scene.h"

#include "solid.h"
#include "actor.h"

constexpr int kMaxEntities = 256;

namespace batteries {

	struct IGameState {
		virtual ~IGameState() = default;
		virtual void Reset(void) = 0;
		virtual void Tick(void) = 0;
		virtual void ForEachActor(const std::function<void(Actor &)> &fn) = 0;
		virtual void ForEachSolid(const std::function<void(Solid &)> &fn) = 0;
	};

	template<typename... EntityTypes>
	struct GameState : IGameState {
		batteries::Scene<kMaxEntities, EntityTypes...> *CurrentScene;

		void ForEachActor(const std::function<void(Actor &)> &fn) override {
			for (auto *actor : CurrentScene->template GetEntitiesOfType<Actor>()) {
				if (actor) fn(*actor);
			}
		}

		void ForEachSolid(const std::function<void(Solid &)> &fn) override {
			for (auto *solid : CurrentScene->template GetEntitiesOfType<Solid>()) {
				if (solid) fn(*solid);
			}
		}

		void Reset(void) override {
			CurrentScene->Reset();
		}

		void Tick(void) override {
			CurrentScene->Update();
			CurrentScene->Render();
		}
	};

}// namespace batteries

batteries::IGameState *World();
batteries::IScene *Scene();