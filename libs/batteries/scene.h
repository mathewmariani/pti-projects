#pragma once

#include "coordinate.h"
#include "registry.h"

#include <vector>

struct Actor;
struct Solid;

namespace batteries {

	struct IScene {
		virtual ~IScene() = default;
		virtual void ForEachActor(const std::function<void(Actor &)> &fn) = 0;
		virtual void ForEachSolid(const std::function<void(Solid &)> &fn) = 0;
	};


	template<size_t Max, typename... Types>
	struct Scene : IScene {
		void Reset() {
			entities.Clear();
		}

		void ForEachActor(const std::function<void(Actor &)> &fn) override {
			for (auto *actor : entities.template GetList<Actor>()) {
				if (actor) fn(*actor);
			}
		}

		void ForEachSolid(const std::function<void(Solid &)> &fn) override {
			for (auto *solid : entities.template GetList<Solid>()) {
				if (solid) fn(*solid);
			}
		}

		template<typename T, typename... Args>
		EntityBase *CreateEntity(Args &&...args) {
			return entities.template Create<T>(std::forward<Args>(args)...);
		}

		void RemoveEntity(EntityBase *entity) {
			if (entity) {
				entities.RemoveAt(entity->id);
			}
		}

		template<typename T>
		std::vector<T *> GetEntitiesOfType() {
			return entities.template GetList<T>();
		}

		template<typename T>
		std::vector<T *> GetCollisions(EntityBase *subject, const CoordXY<int> &dir) {
			std::vector<T *> result;
			result.reserve(256);

			entities.template ForEach<T>([&](T *other) {
				if (subject->Overlaps(other, dir)) {
					result.push_back(other);
				}
			});
			return result;
		}

		template<typename T>
		void UpdateEntitiesOfType() {
			entities.template ForEach<T>([](T *e) {
				e->timer += (1.0 / 30.0);
				e->Update();
				e->Physics();
			});
		}

		template<typename T>
		void RenderEntitiesOfType() {
			entities.template ForEach<T>([](T *e) {
				e->Render();
			});
		}

		virtual void Init(void) {};
		virtual void Update(void) {};
		virtual void Render(void) {};

	private:
		EntityManager<Max, Types...> entities;
	};

}// namespace batteries