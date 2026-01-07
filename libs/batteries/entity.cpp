#include "pti/pti.h"

#include "entity.h"
#include "registry.h"
#include <algorithm>

#include "gamestate.h"

#define kTileSize (Scene()->GetTileSize())

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

bool EntityBase::PlaceMeetingForTile(int i, int j, const CoordXY<int> &dir, uint8_t flags) const {
	switch ((Collisions)flags) {
		case Collisions::None:
			return false;
		case Collisions::Solid:
			return true;		
		case Collisions::Semi:
			return ((position.y + by + bh - speed.y) <= (j * kTileSize));
		case Collisions::SlopeRB: {// Shallow slope bottom (right)
			float cx = (position.x + bx + bw + dir.x) - i * kTileSize;
			float slope_y = (j + 1) * kTileSize - 0.5f * cx;
			return (position.y + by + bh + dir.y > slope_y);
		}
		case Collisions::SlopeRT: {// Shallow slope top (right)
			float cx = (position.x + bx + bw + dir.x) - i * kTileSize;
			float slope_y = (j + 1) * kTileSize - 4 - 0.5f * cx;
			return (position.y + by + bh + dir.y > slope_y);
		}
		case Collisions::SlopeLT: {// Shallow slope top (left)
			float cx = (position.x + bx + dir.x) - i * kTileSize;
			float slope_y = j * kTileSize + 0.5f * cx;
			return (position.y + by + bh + dir.y > slope_y);
		}
		case Collisions::SlopeLB: {// Shallow slope bottom (left)
			float cx = (position.x + bx + dir.x) - i * kTileSize;
			float slope_y = (j + 1) * kTileSize - 4 + 0.5f * cx;
			return (position.y + by + bh + dir.y > slope_y);
		}
		case Collisions::SpikeT: {// top-half
			float solid_y = j * kTileSize + 4;
			return (position.y + by + dir.y < solid_y);
		}
		case Collisions::SpikeR: {// right-half
			float solid_x = (i + 1) * kTileSize - 4;
			return (position.x + bx + bw + dir.x > solid_x);
		}
		case Collisions::SpikeB: {// bottom-half (one-way)
			float solid_y = (j + 1) * kTileSize - 4;
			return (position.y + by + bh + dir.y > solid_y);
		}
		case Collisions::SpikeL: {// left-half
			float solid_x = i * kTileSize + 4;
			return (position.x + bx + dir.x < solid_x);
		}
	}
	return true;
}

template<typename Predicate>
bool EntityBase::CheckTiles(const CoordXY<int> &dir, Predicate &&pred) const {
	const auto top = std::max(0, (position.y + by + dir.y) / kTileSize);
	const auto left = std::max(0, (position.x + bx + dir.x) / kTileSize);
	const auto bottom = (position.y + by + bh + dir.y - 1) / kTileSize;
	const auto right = (position.x + bx + bw + dir.x - 1) / kTileSize;

	for (int j = top; j <= bottom; ++j) {
		for (int i = left; i <= right; ++i) {
			int tile = pti_mget(i, j);
			uint8_t flags = pti_fget(tile);
			if (pred(i, j, flags)) {
				return true;
			}
		}
	}
	return false;
}

bool EntityBase::PlaceMeeting(const CoordXY<int> &dir) const {
	return CheckTiles(dir, [&](int i, int j, uint8_t flags) {
		return PlaceMeetingForTile(i, j, dir, flags);
	});
}

bool EntityBase::PlaceMeeting(const CoordXY<int> &dir, int index) const {
	return CheckTiles(dir, [&](int i, int j, uint8_t flags) {
		if (flags != index) {
			return false;
		}
		return PlaceMeetingForTile(i, j, dir, flags);
	});
}

void EntityBase::Destroy(void) {
	Scene()->RemoveEntity(this);
}