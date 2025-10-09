#pragma once

#include "base.h"

struct Solid : EntityBase {
	static constexpr auto cEntityType = EntityType::Solid;

	void Physics(void);

	void MoveX(float amount);
	void MoveY(float amount);
};