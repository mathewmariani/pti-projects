#include "pti/pti.h"
#include "solid.h"
#include "actor.h"
#include "gamestate.h"

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
	return Overlaps(base, CoordXY<int>::Up);
}