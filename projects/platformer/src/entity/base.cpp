#include "pti/pti.h"

#include "base.h"
#include "../bank.h"
#include "registry.h"

#include <algorithm>

constexpr int EN_TILE_SIZE = 8;
constexpr int EN_ROOM_WIDTH = 512;
constexpr int EN_ROOM_HEIGHT = 288;
constexpr int EN_ROOM_COLS = EN_ROOM_WIDTH / EN_TILE_SIZE;
constexpr int EN_ROOM_ROWS = EN_ROOM_HEIGHT / EN_TILE_SIZE;

enum class Collisions : uint8_t {
	None = 0x00,
	Solid = 0x01,
	Semi = 0x02,
	SpikeT = 0x03,
	SpikeR = 0x04,
	SpikeB = 0x05,
	SpikeL = 0x06,
	SlopeRB = 0x07,
	SlopeRT = 0x08,
	SlopeLT = 0x09,
	SlopeLB = 0x0A,
};

template<>
bool EntityBase::Is<EntityBase>() const {
	return true;
}

void EntityBase::Step() {
	Update();
	Physics();
}

void EntityBase::Physics() {
	timer += PTI_DELTA;

	/* Get move amount */
	int nx = sx + rx;
	int ny = sy + ry;

	/* Update move remainder */
	rx = rx + sx;
	ry = ry + sy;

	/* Move in X direction */
	int j = std::round(rx);
	int dx = _pti_sign(j);
	rx -= j;
	while (j != 0) {
		// Moving up slope:
		if (PlaceMeeting({dx, 0}) && !(PlaceMeeting({dx, -1}))) {
			y -= 1;
		}
		// Moving down slope:
		if (!(PlaceMeeting({dx, 0})) && !(PlaceMeeting({dx, 1})) && PlaceMeeting({dx, 2})) {
			y += 1;
		}

		// Always last
		if (PlaceMeeting({dx, 0})) {
			sx = rx = nx = 0;
			break;
		}

		// check for entity
		EntityBase *other = nullptr;
		if (CheckCollisionsWith(this, other, {dx, 0})) {
			if (PreSolidCollisionWith(other, {dx, 0}) || other->ProvidesStaticCollision()) {
				sx = rx = nx = 0;
				break;
			}
		}

		x += dx;
		j -= dx;
	}

	/* Move in X direction */
	j = std::round(ry);
	int dy = _pti_sign(j);
	ry -= j;
	while (j != 0) {
		if (sy < 0 && !CanWiggle()) {
			sy = ry = ny = 0;
			break;
		}
		if (PlaceMeeting({0, dy})) {
			flags |= EntityFlags::ENTITYFLAG_GROUNDED;
			sy = ry = ny = 0;
			break;
		}
		// check for entity
		EntityBase *other = nullptr;
		if (CheckCollisionsWith(this, other, {0, dy})) {
			if (PreSolidCollisionWith(other, {0, dy}) || other->ProvidesStaticCollision()) {
				flags |= EntityFlags::ENTITYFLAG_GROUNDED;
				sy = ry = ny = 0;
				break;
			}
		}
		flags &= EntityFlags::ENTITYFLAG_GROUNDED;
		y += dy;
		j -= dy;
	}
}

void EntityBase::Update() {}
void EntityBase::PostUpdate() {}
void EntityBase::Render() {}

bool EntityBase::PreSolidCollisionWith(EntityBase *const other, const CoordXY<int> &dir) {
	return false;
}

const EntityReaction EntityBase::Interact(const EntityInteraction interaction, EntityBase *const from, const CoordXY<int> &dir) {
	return EntityReaction::None;
}

void EntityBase::SetLocation(const CoordXY<int> &newLocation) {
	x = newLocation.x;
	y = newLocation.y;
}

bool EntityBase::IsGrounded() const {
	return (flags & EntityFlags::ENTITYFLAG_GROUNDED) != 0;
}

bool EntityBase::ProvidesStaticCollision() const {
	return (flags & EntityFlags::ProvidesStaticCollision) != 0;
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
			auto tile = pti_mget(i, j);
			auto flags = pti_fget(tile);
			switch ((Collisions)flags) {
				case Collisions::Solid:
					return true;
				case Collisions::Semi:
					if ((y + by + bh - sy) <= (j * EN_TILE_SIZE)) return true;
					continue;
				case Collisions::SlopeRB: {// Shallow slope bottom (right)
					float cx = (x + bx + bw + dir.x) - i * EN_TILE_SIZE;
					float slope_y = (j + 1) * EN_TILE_SIZE - 0.5f * cx;
					if (y + by + bh + dir.y > slope_y) return true;
					continue;
				}
				case Collisions::SlopeRT: {// Shallow slope top (right)
					float cx = (x + bx + bw + dir.x) - i * EN_TILE_SIZE;
					float slope_y = (j + 1) * EN_TILE_SIZE - 4 - 0.5f * cx;
					if (y + by + bh + dir.y > slope_y) return true;
					continue;
				}
				case Collisions::SlopeLT: {// Shallow slope top (left)
					float cx = (x + bx + dir.x) - i * EN_TILE_SIZE;
					float slope_y = j * EN_TILE_SIZE + 0.5f * cx;
					if (y + by + bh + dir.y > slope_y) return true;
					continue;
				}
				case Collisions::SlopeLB: {// Shallow slope bottom (left)
					float cx = (x + bx + dir.x) - i * EN_TILE_SIZE;
					float slope_y = (j + 1) * EN_TILE_SIZE - 4 + 0.5f * cx;
					if (y + by + bh + dir.y > slope_y) return true;
					continue;
				}
				case Collisions::SpikeT: {// top-half
					float solid_y = j * EN_TILE_SIZE + 4;
					if (y + by + dir.y < solid_y) return true;
					continue;
				}
				case Collisions::SpikeR: {// right-half
					float solid_x = (i + 1) * EN_TILE_SIZE - 4;
					if (x + bx + bw + dir.x > solid_x) return true;
					continue;
				}
				case Collisions::SpikeB: {// bottom-half (one-way)
					float solid_y = (j + 1) * EN_TILE_SIZE - 4;
					if (y + by + bh + dir.y > solid_y) return true;
					continue;
				}
				case Collisions::SpikeL: {// left-half
					float solid_x = i * EN_TILE_SIZE + 4;
					if (x + bx + dir.x < solid_x) return true;
					continue;
				}

				case Collisions::None:
				default:
					continue;
			}
		}
	}

	return false;
}

bool EntityBase::IsTouching() const {
	return PlaceMeeting({direction, 0});
}

bool EntityBase::CanWiggle() {
	for (auto i = 0; i < 4; ++i) {
		for (auto j : {-1, 1}) {
			if (!PlaceMeeting({i * j, -1})) {
				x += (i * j);
				return true;
			}
		}
	}
	return false;
}