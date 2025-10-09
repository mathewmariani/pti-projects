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
	sx = kPlatformAcceleration * direction * PTI_DELTA;
	float next_x = x + sx;

	if (next_x > start_x + kPlatformRange) {
		sx = (start_x + kPlatformRange) - x;
		direction = -1.0f;
	} else if (next_x < start_x) {
		sx = (start_x) -x;
		direction = 1.0f;
	}
}


void Platform::Render() {
	auto color = 0xffff0000;
	pti_rectf(x, y, x + kPlatformWidth - 1, y + kPlatformHeight - 1, color);
}