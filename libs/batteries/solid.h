#pragma once

#include "entity.h"

struct Solid : EntityBase {
	void Physics(void);

	void MoveX(float amount);
	void MoveY(float amount);
};