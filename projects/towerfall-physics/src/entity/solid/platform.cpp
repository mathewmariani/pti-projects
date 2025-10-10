#include "pti/pti.h"

#include "platform.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"

#include <cmath>

template<>
bool EntityBase::Is<Platform>() const {
	return type == EntityType::Solid;
}

void Platform::Update() {
	switch (type) {
		case Platform::Type::Horizontal: {
			sx = kPlatformAcceleration * direction.x * PTI_DELTA;
			float next_x = x + sx;

			if (next_x > start_x + kPlatformRange) {
				sx = (start_x + kPlatformRange) - x;
				direction.x = -1;
			} else if (next_x < start_x) {
				sx = (start_x) -x;
				direction.x = 1;
			}
		} break;
		case Platform::Type::Vertical: {
			sy = kPlatformAcceleration * direction.y * PTI_DELTA;
			float next_y = y + sy;

			if (next_y < start_y - kPlatformRange) {
				sy = (start_y - kPlatformRange) - y;
				direction.y = +1;
			} else if (next_y > start_y) {
				sy = (start_y) -y;
				direction.y = -1;
			}
		} break;
	}
}

void Platform::Render() {
	pti_spr(bitmap_platform, 0, x, y, false, false);
}