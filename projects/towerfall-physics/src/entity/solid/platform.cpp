#include "pti/pti.h"

#include "platform.h"
#include "../../gamestate.h"
#include "../../bank.h"

void Platform::Update() {
	switch (type) {
		case Platform::Type::Horizontal: {
			speed.x = kPlatformAcceleration * direction.x * PTI_DELTA;
			float next_x = position.x + speed.x;

			if (next_x > start_x + kPlatformRange) {
				speed.x = (start_x + kPlatformRange) - position.x;
				direction.x = -1;
			} else if (next_x < start_x) {
				speed.x = (start_x) -position.x;
				direction.x = 1;
			}
		} break;
		case Platform::Type::Vertical: {
			speed.y = kPlatformAcceleration * direction.y * PTI_DELTA;
			float next_y = position.y + speed.y;

			if (next_y < start_y - kPlatformRange) {
				speed.y = (start_y - kPlatformRange) - position.y;
				direction.y = +1;
			} else if (next_y > start_y) {
				speed.y = (start_y) -position.y;
				direction.y = -1;
			}
		} break;
	}
}

void Platform::Render() {
	pti_spr(bitmap_platform, 0, position.x, position.y, false, false);
}