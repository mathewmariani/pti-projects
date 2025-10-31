#pragma once

#include <variant>
#include <array>
#include <vector>
#include <cstddef>
#include <type_traits>
#include <functional>
#include <utility>
#include <optional>

// Example entity base
#include "entity.h"// defines EntityBase, Actor, Player, etc.

template<std::size_t MaxEntities, typename... Ts>
class EntityManager {
public:
	using EntityVariant = std::variant<std::monostate, Ts...>;

	EntityManager() { initializeFreeIds(); }
	~EntityManager() = default;

	// Create a new entity of type T
	template<typename T, typename... Args>
	T *Create(Args &&...args) {
		static_assert((std::disjunction_v<std::is_same<T, Ts>...>),
					  "T must be one of the EntityManager's managed types");

		if (freeIds_.empty())
			throw std::runtime_error("EntityManager is full");

		size_t index = freeIds_.back();
		freeIds_.pop_back();

		entities_[index].template emplace<T>(std::forward<Args>(args)...);
		auto &entity = std::get<T>(entities_[index]);
		entity.id = index;
		entity.timer = 0.0f;
		return &entity;
	}

	// Remove entity at index
	bool RemoveAt(size_t index) {
		if (index >= MaxEntities) return false;

		entities_[index] = std::monostate{};
		freeIds_.push_back(index);
		return true;
	}

	// Remove entities by exact type and predicate
	template<typename T, typename Predicate>
	void RemoveIf(Predicate &&pred) {
		for (size_t i = 0; i < MaxEntities; ++i) {
			auto &e = entities_[i];
			if (auto ptr = std::get_if<T>(&e)) {
				if (pred(*ptr)) {
					e = std::monostate{};
					freeIds_.push_back(i);
				}
			}
		}
	}

	// Iterate over entities of exact type
	template<typename T, typename Func>
	void ForEach(Func &&f) {
		std::vector<T *> result;
		for (auto &e : entities_) {
			// clang-format off
			std::visit([&](auto &val) {
				using U = std::decay_t<decltype(val)>;
				if constexpr (!std::is_same_v<U, std::monostate>) {
					if (auto *ptr = dynamic_cast<T *>(&val)) {
						f(ptr);
					}
				}
			}, e);
			// clang-format on
		}
	}

	// Get all entities of a specific type
	template<typename T>
	std::vector<T *> GetList() {
		std::vector<T *> result;
		for (auto &e : entities_) {
			// clang-format off
			std::visit([&](auto &val) {
				using U = std::decay_t<decltype(val)>;
				if constexpr (!std::is_same_v<U, std::monostate>) {
					if (auto *ptr = dynamic_cast<T *>(&val)) {
						result.push_back(ptr);
					}
				}
			}, e);
			// clang-format on
		}
		return result;
	}


	// Clear all entities
	void Clear() {
		for (auto &e : entities_) {
			e = std::monostate{};
		}
		freeIds_.clear();
		initializeFreeIds();
	}

	const std::array<EntityVariant, MaxEntities> &EntitiesArray() const {
		return entities_;
	}

private:
	std::array<EntityVariant, MaxEntities> entities_{};
	std::vector<size_t> freeIds_{};

	void initializeFreeIds() {
		freeIds_.clear();
		for (size_t i = 0; i < MaxEntities; ++i)
			freeIds_.push_back(MaxEntities - 1 - i);
	}
};
