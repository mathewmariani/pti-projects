#include "solid.h"
#include "registry.h"
#include "../gamestate.h"
#include "../bank.h"
#include "pti/pti.h"

#include <cmath>

void Actor::Physics() {
	MoveX(speed.x, &Actor::HaltX);
	MoveY(speed.y, &Actor::HaltY);

	const auto dir = CoordXY<int>::Up;
	grounded = PlaceMeeting(dir) || CollidesWithSolids(dir);
}

void Actor::MoveX(float amount, Actor::MoveFunc func = nullptr) {
	remainder.x += amount;
	int move = std::round(remainder.x);
	if (move != 0) {
		const int dx = _pti_sign(move);
		const CoordXY<int> dir{dx, 0};
		while (move != 0) {
			if (!PlaceMeeting(dir) && !CollidesWithSolids(dir)) {
				position.x += dx;
				move -= dx;
			} else {
				if (func) {
					(this->*func)();
				}
				break;
			}
		}
		remainder.x = 0;
	}
}

void Actor::MoveY(float amount, Actor::MoveFunc func = nullptr) {
	remainder.y += amount;
	int move = std::round(remainder.y);
	if (move != 0) {
		const int dy = _pti_sign(move);
		const CoordXY<int> dir{0, dy};
		while (move != 0) {
			if (!PlaceMeeting(dir) && !CollidesWithSolids(dir)) {
				position.y += dy;
				move -= dy;
			} else {
				if (func) {
					(this->*func)();
				}
				break;
			}
		}
		remainder.y = 0;
	}
}

bool Actor::CanWiggle() {
	for (auto i = 0; i < 4; ++i) {
		for (auto j : {-1, 1}) {
			const CoordXY<int> dir{i * j, -1};
			if (!PlaceMeeting(dir) && !CollidesWithSolids(dir)) {
				position.x += (i * j);
				return true;
			}
		}
	}
	return false;
}

void Actor::Squish(void) {
	RemoveEntity(this);
	GetGameState().Deaths++;
	GetGameState().PlayerIsDead = true;
}

void Actor::HaltX(void) {
	speed.x = 0;
}

void Actor::HaltY(void) {
	speed.y = 0;
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