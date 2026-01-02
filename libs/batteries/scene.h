#pragma once

#include "coordinate.h"
#include "registry.h"

#include <vector>
#include "entity.h"

struct Actor;
struct Solid;

namespace batteries {

	struct IScene {
		virtual ~IScene() = default;
		virtual void Reset(void) = 0;
		virtual void Init(void) = 0;
		virtual void Update(void) = 0;
		virtual void Render(void) = 0;
		virtual void RemoveEntity(EntityBase *entity) = 0;
		virtual void ForEachActor(const std::function<void(Actor &)> &fn) = 0;
		virtual void ForEachSolid(const std::function<void(Solid &)> &fn) = 0;
	};

	template<size_t Max, typename... Types>
	struct Scene : IScene {
		void Reset() override {
			entities.Clear();
		}

		virtual void Init(void) override {};
		virtual void Update(void) override {};
		virtual void Render(void) override {};

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

		void RemoveEntity(EntityBase *entity) override {
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

	private:
		EntityManager<Max, Types...> entities;
	};

}// namespace batteries