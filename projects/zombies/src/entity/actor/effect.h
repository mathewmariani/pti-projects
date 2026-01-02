#pragma once

#include "batteries/actor.h"

struct Effect : Actor {
	enum class Type : uint8_t {
		Collect,
	};

	static void Create(const CoordXY<int> &location, Type type = Type::Collect);

	Effect();

	void Update() override;
	void Render() override;

	Type type;
};