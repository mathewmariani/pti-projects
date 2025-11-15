#pragma once

#include "batteries/entity.h"

namespace batteries {

	struct Solid : EntityBase {
		void Physics(void);

		void MoveX(float amount);
		void MoveY(float amount);
	};

}// namespace batteries