#include "solid.h"
#include "registry.h"
#include "../gamestate.h"
#include "../bank.h"
#include "pti/pti.h"

#include <cmath>

template<>
bool EntityBase::Is<Actor>() const {
	return type == EntityType::Actor;
}

void Actor::Physics() {
	MoveX(sx, &Actor::HaltX);
	MoveY(sy, &Actor::HaltY);
}

void Actor::MoveX(float amount, Actor::MoveFunc func = nullptr) {
	rx += amount;
	int move = std::round(rx);
	if (move != 0) {
		const int dx = _pti_sign(move);
		const CoordXY<int> dir{dx, 0};
		while (move != 0) {
			if (!PlaceMeeting(dir) && !CollidesWithSolids(dir)) {
				x += dx;
				move -= dx;
			} else {
				if (func) {
					(this->*func)();
				}
				break;
			}
		}
		rx = 0;
	}
}

void Actor::MoveY(float amount, Actor::MoveFunc func = nullptr) {
	ry += amount;
	int move = std::round(ry);
	if (move != 0) {
		const int dy = _pti_sign(move);
		const CoordXY<int> dir{0, dy};
		while (move != 0) {
			if (!PlaceMeeting(dir) && !CollidesWithSolids(dir)) {
				y += dy;
				move -= dy;
			} else {
				if ((dy < 0 && !CanWiggle()) && func) {
					(this->*func)();
				}
				break;
			}
		}
		ry = 0;
	}
}

bool Actor::CanWiggle() {
	for (auto i = 0; i < 4; ++i) {
		for (auto j : {-1, 1}) {
			const CoordXY<int> dir{i * j, -1};
			if (!PlaceMeeting(dir) && !CollidesWithSolids(dir)) {
				x += (i * j);
				return true;
			}
		}
	}
	return false;
}

void Actor::Squish(void) {
	printf("%s\n", __FUNCTION__);
}

void Actor::HaltX(void) {
	sx = 0;
}

void Actor::HaltY(void) {
	sy = 0;
}

bool Actor::IsRidding(const EntityBase *base) const {
	return Overlaps(base, {0, 0}) || Overlaps(base, {0, 1});
}

bool Actor::IsGrounded(void) const {
	const CoordXY<int> dir{0, 1};
	return PlaceMeeting(dir) || CollidesWithSolids(dir);
}

bool Actor::CollidesWithSolids(const CoordXY<int> &dir) const {
	for (auto &e : GetGameState().Entities) {
		// clang-format off
		auto collided = std::visit([&](auto &obj) {
			using U = std::decay_t<decltype(obj)>;
			if constexpr (std::is_base_of_v<Solid, U>) {
				return this->Overlaps(&obj, dir);
			}
      return false;
		}, e);
		// clang-format on
		if (collided) {
			return true;
		}
	}
	return false;
}