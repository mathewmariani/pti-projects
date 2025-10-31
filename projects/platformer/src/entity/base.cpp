#include "pti/pti.h"

#include "base.h"
#include "../bank.h"
#include "registry.h"

#include <algorithm>


constexpr int EN_TILE_SIZE = 8;
constexpr int EN_ROOM_WIDTH = 512;
constexpr int EN_ROOM_HEIGHT = 368;
constexpr int EN_ROOM_COLS = EN_ROOM_WIDTH / EN_TILE_SIZE;
constexpr int EN_ROOM_ROWS = EN_ROOM_HEIGHT / EN_TILE_SIZE;

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

	/* Move in X direction */
	int j = nx;
	int dx = _pti_sign(j);
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

	// Move in Y direction
	j = ny;
	int dy = _pti_sign(j);
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

	/* Update move remainder */
	rx = sx + rx - nx;
	ry = sy + ry - ny;
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
			auto flags = pti_fget(i, j);

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

bool EntityBase::IsTouching() const {
	return PlaceMeeting({direction, 0});
}

bool EntityBase::IsTouchingWall() const {
	const auto top = std::max(0, (y + by) / EN_TILE_SIZE);
	const auto left = std::max(0, (x + bx + direction) / EN_TILE_SIZE);
	const auto bottom = std::min(EN_ROOM_ROWS - 1, (y + by + bh - 1) / EN_TILE_SIZE);
	const auto right = std::min(EN_ROOM_COLS - 1, (x + bx + bw + direction - 1) / EN_TILE_SIZE);

	for (auto j = top; j <= bottom; ++j) {
		for (auto i = left; i <= right; ++i) {
			auto flags = pti_fget(i, j);
			switch (flags) {
				// --- Non-colliding tiles ---
				case 0:
				case 46:
				case 47:
					continue;

				// --- Slopes (walkable, not walls) ---
				case 31:
				case 32:
				case 33:
				case 34:
					continue;

				// --- One-way platforms (also not walls) ---
				case 27:
				case 28:
				case 29:
				case 30:
				case 38:
				case 39:
				case 40:
				case 41:
					continue;

				// --- Solid tiles (true walls) ---
				default:
					return true;
			}
		}
	}

	return false;
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