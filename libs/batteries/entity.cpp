#include "pti/pti.h"

#include "entity.h"
#include "registry.h"
#include <algorithm>

#include "gamestate.h"

constexpr int kScreenWidth = 176;
constexpr int kScreenHeight = 128;
constexpr int kTileSize = 8;
constexpr int EN_ROOM_WIDTH = (176 * 3);
constexpr int EN_ROOM_HEIGHT = (128 * 3);
constexpr int EN_ROOM_COLS = EN_ROOM_WIDTH / kTileSize;
constexpr int EN_ROOM_ROWS = EN_ROOM_HEIGHT / kTileSize;

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

bool EntityBase::PlaceMeetingForTile(int i, int j, const CoordXY<int> &dir, int flags) const {
	switch (flags) {
		// Non-colliding tiles
		case 0:
		case 46:
		case 47:
			return false;

		// Slopes
		case 31: {// Shallow slope bottom (right)
			float cx = (position.x + bx + bw + dir.x) - i * kTileSize;
			float slope_y = (j + 1) * kTileSize - 0.5f * cx;
			return (position.y + by + bh + dir.y > slope_y);
		}
		case 32: {// Shallow slope top (right)
			float cx = (position.x + bx + bw + dir.x) - i * kTileSize;
			float slope_y = (j + 1) * kTileSize - 4 - 0.5f * cx;
			return (position.y + by + bh + dir.y > slope_y);
		}
		case 33: {// Shallow slope top (left)
			float cx = (position.x + bx + dir.x) - i * kTileSize;
			float slope_y = j * kTileSize + 0.5f * cx;
			return (position.y + by + bh + dir.y > slope_y);
		}
		case 34: {// Shallow slope bottom (left)
			float cx = (position.x + bx + dir.x) - i * kTileSize;
			float slope_y = (j + 1) * kTileSize - 4 + 0.5f * cx;
			return (position.y + by + bh + dir.y > slope_y);
		}

		// One-way platforms
		case 27:
		case 28:
		case 29:
		case 30:
		case 38:
		case 39:
		case 40:
		case 41:
			return ((position.y + by + bh - speed.y) <= (j * kTileSize));

		// Half-tiles
		case 42: {// top-half
			float solid_y = j * kTileSize + 4;
			return (position.y + by + dir.y < solid_y);
		}
		case 43: {// right-half
			float solid_x = (i + 1) * kTileSize - 4;
			return (position.x + bx + bw + dir.x > solid_x);
		}
		case 53: {// bottom-half (one-way)
			float solid_y = (j + 1) * kTileSize - 4;
			return (position.y + by + bh + dir.y > solid_y);
		}
		case 54: {// left-half
			float solid_x = i * kTileSize + 4;
			return (position.x + bx + dir.x < solid_x);
		}

		// Solid tiles
		default:
			return true;
	}
}

template<typename Predicate>
bool EntityBase::CheckTiles(const CoordXY<int> &dir, Predicate &&pred) const {
	const auto top = std::max(0, (position.y + by + dir.y) / kTileSize);
	const auto left = std::max(0, (position.x + bx + dir.x) / kTileSize);
	const auto bottom = std::min(EN_ROOM_ROWS - 1, (position.y + by + bh + dir.y - 1) / kTileSize);
	const auto right = std::min(EN_ROOM_COLS - 1, (position.x + bx + bw + dir.x - 1) / kTileSize);

	for (int j = top; j <= bottom; ++j) {
		for (int i = left; i <= right; ++i) {
			int tile = pti_mget(i, j);
			int flags = pti_fget(tile);
			if (pred(i, j, flags)) {
				return true;
			}
		}
	}
	return false;
}

bool EntityBase::PlaceMeeting(const CoordXY<int> &dir) const {
	return CheckTiles(dir, [&](int i, int j, int flags) {
		return PlaceMeetingForTile(i, j, dir, flags);
	});
}

bool EntityBase::PlaceMeeting(const CoordXY<int> &dir, int index) const {
	return CheckTiles(dir, [&](int i, int j, int flags) {
		if (flags != index) {
			return false;
		}
		return PlaceMeetingForTile(i, j, dir, flags);
	});
}

void EntityBase::Destroy(void) {
	Scene()->RemoveEntity(this);
}