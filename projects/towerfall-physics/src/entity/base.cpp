#include "pti/pti.h"

#include "base.h"
#include "../bank.h"
#include "registry.h"

#include "../gamestate.h"
#include <algorithm>

template<>
bool EntityBase::Is<EntityBase>() const {
	return true;
}

void EntityBase::Update() {}
void EntityBase::Render() {}
void EntityBase::Physics() {}

void EntityBase::SetLocation(const CoordXY<int> &newLocation) {
	x = newLocation.x;
	y = newLocation.y;
	start_x = x;
	start_y = y;
}

bool EntityBase::Overlaps(const EntityBase *other, const CoordXY<int> &dir) const {
	return (x + bx + dir.x < other->x + other->bx + other->bw) &&
		   (y + by + dir.y < other->y + other->by + other->bh) &&
		   (x + bx + bw + dir.x > other->x + other->bx) &&
		   (y + by + bh + dir.y > other->y + other->by);
}

bool EntityBase::PlaceMeeting(const CoordXY<int> &dir) const {
	const auto top = std::max(0, (y + by + dir.y) / EN_TILE_SIZE);
	const auto left = std::max(0, (x + bx + dir.x) / EN_TILE_SIZE);
	const auto bottom = std::min(EN_ROOM_ROWS - 1, (y + by + bh + dir.y - 1) / EN_TILE_SIZE);
	const auto right = std::min(EN_ROOM_COLS - 1, (x + bx + bw + dir.x - 1) / EN_TILE_SIZE);

	for (auto j = top; j <= bottom; ++j) {
		for (auto i = left; i <= right; ++i) {
			auto flags = pti_fget(tilemap, i, j);

			switch (flags) {
				// Non-colliding tiles
				case 0:
				case 46:
				case 47:
					continue;

				// Slope handling
				case 31: {// Shallow slope bottom (right)
					auto cx = (x + bx + bw + dir.x) - i * EN_TILE_SIZE;
					auto slope_y = (j + 1) * EN_TILE_SIZE - 0.5f * cx;
					if (y + by + bh + dir.y > slope_y) return true;
					continue;
				}
				case 32: {// Shallow slope top (right)
					auto cx = (x + bx + bw + dir.x) - i * EN_TILE_SIZE;
					auto slope_y = (j + 1) * EN_TILE_SIZE - 4 - 0.5f * cx;
					if (y + by + bh + dir.y > slope_y) return true;
					continue;
				}
				case 33: {// Shallow slope top (left)
					auto cx = (x + bx + dir.x) - i * EN_TILE_SIZE;
					auto slope_y = j * EN_TILE_SIZE + 0.5f * cx;
					if (y + by + bh + dir.y > slope_y) return true;
					continue;
				}
				case 34: {// Shallow slope bottom (left)
					auto cx = (x + bx + dir.x) - i * EN_TILE_SIZE;
					auto slope_y = (j + 1) * EN_TILE_SIZE - 4 + 0.5f * cx;
					if (y + by + bh + dir.y > slope_y) return true;
					continue;
				}

				// One-way platforms
				case 27:
				case 28:
				case 29:
				case 30:
				case 38:
				case 39:
				case 40:
				case 41: {
					// Allow collision only if the entity was entirely above the platform before the movement
					if ((y + by + bh - sy) <= (j * EN_TILE_SIZE)) return true;
					continue;
				}

				// Solid tiles
				default:
					return true;
			}
		}
	}

	return false;
}
