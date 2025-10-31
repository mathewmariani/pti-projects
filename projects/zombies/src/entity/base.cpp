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
	position = newLocation;
	start_x = newLocation.x;
	start_y = newLocation.y;
}

bool EntityBase::Overlaps(const EntityBase *other, const CoordXY<int> &dir) const {
	return (position.x + bx + dir.x < other->position.x + other->bx + other->bw) &&
		   (position.y + by + dir.y < other->position.y + other->by + other->bh) &&
		   (position.x + bx + bw + dir.x > other->position.x + other->bx) &&
		   (position.y + by + bh + dir.y > other->position.y + other->by);
}

bool EntityBase::PlaceMeeting(const CoordXY<int> &dir) const {
	const auto top = std::max(0, (position.y + by + dir.y) / kTileSize);
	const auto left = std::max(0, (position.x + bx + dir.x) / kTileSize);
	const auto bottom = std::min(EN_ROOM_ROWS - 1, (position.y + by + bh + dir.y - 1) / kTileSize);
	const auto right = std::min(EN_ROOM_COLS - 1, (position.x + bx + bw + dir.x - 1) / kTileSize);

	for (auto j = top; j <= bottom; ++j) {
		for (auto i = left; i <= right; ++i) {
			auto flags = pti_fget(i, j);

			switch (flags) {
				// Non-colliding tiles
				case 0:
				case 46:
				case 47:
					continue;

				// Slope handling
				case 31: {// Shallow slope bottom (right)
					auto cx = (position.x + bx + bw + dir.x) - i * kTileSize;
					auto slope_y = (j + 1) * kTileSize - 0.5f * cx;
					if (position.y + by + bh + dir.y > slope_y) return true;
					continue;
				}
				case 32: {// Shallow slope top (right)
					auto cx = (position.x + bx + bw + dir.x) - i * kTileSize;
					auto slope_y = (j + 1) * kTileSize - 4 - 0.5f * cx;
					if (position.y + by + bh + dir.y > slope_y) return true;
					continue;
				}
				case 33: {// Shallow slope top (left)
					auto cx = (position.x + bx + dir.x) - i * kTileSize;
					auto slope_y = j * kTileSize + 0.5f * cx;
					if (position.y + by + bh + dir.y > slope_y) return true;
					continue;
				}
				case 34: {// Shallow slope bottom (left)
					auto cx = (position.x + bx + dir.x) - i * kTileSize;
					auto slope_y = (j + 1) * kTileSize - 4 + 0.5f * cx;
					if (position.y + by + bh + dir.y > slope_y) return true;
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
					if ((position.y + by + bh - speed.y) <= (j * kTileSize)) return true;
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
