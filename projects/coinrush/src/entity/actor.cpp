#include "pti/pti.h"
#include "solid.h"
#include "actor.h"
#include "../gamestate.h"

void Actor::Physics() {
	MoveX(speed.x, &Actor::HaltX);
	MoveY(speed.y, &Actor::HaltY);

	const auto dir = CoordXY<int>::Up;
	grounded = PlaceMeeting(dir) || CollidesWithSolids(dir);
}

void Actor::MoveX(float amount, Actor::MoveFunc func = nullptr) {
	remainder.x += amount;
	int move = _pti_round(remainder.x);
	if (move != 0) {
		direction.x = _pti_sign(move);
		remainder.x -= move;
		const int dx = direction.x;
		const CoordXY<int> dir{dx, 0};
		while (move != 0) {
			// moving up slope:
			if (PlaceMeeting({dx, 0}) && !(PlaceMeeting({dx, -1}))) {
				position.y -= 1;
			}
			// moving down slope:
			if (!(PlaceMeeting({dx, 0})) && !(PlaceMeeting({dx, 1})) && PlaceMeeting({dx, 2})) {
				position.y += 1;
			}
			// always last:
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
	}
}

void Actor::MoveY(float amount, Actor::MoveFunc func = nullptr) {
	remainder.y += amount;
	int move = _pti_round(remainder.y);
	if (move != 0) {
		direction.y = _pti_sign(move);
		remainder.y -= move;
		const int dy = direction.y;
		const CoordXY<int> dir{0, dy};
		while (move != 0) {
			if (!PlaceMeeting(dir) && !CollidesWithSolids(dir)) {
				position.y += dy;
				move -= dy;
			} else {
				auto squished = false;
				if (dy < 0) {
					// moving up:
					squished = !CanWiggle();
				} else if (dy > 0) {
					// moving down:
					squished = (PlaceMeeting({0, -1}) || CollidesWithSolids({0, -1}));
				}
				if (squished && func) {
					(this->*func)();
				}
				break;
			}
		}
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
	/* do nothing */
}

void Actor::HaltX(void) {
	speed.x = 0;
}

void Actor::HaltY(void) {
	speed.y = 0;
}

bool Actor::IsRiding(const EntityBase *base) const {
	if (!CanBeMoved()) { return false; }
	return Overlaps(base, CoordXY<int>::Zero) || Overlaps(base, CoordXY<int>::Up);
}

bool Actor::IsGrounded(void) const {
	const auto dir = CoordXY<int>::Up;
	return PlaceMeeting(dir) || CollidesWithSolids(dir);
}

bool Actor::CollidesWithSolids(const CoordXY<int> &dir) const {
	auto collided = false;
	GetGameState().Entities.ForEach<Solid>([&](Solid *solid) {
		if (this->Overlaps(solid, dir)) {
			collided = true;
			return;
		}
	});
	return collided;
}