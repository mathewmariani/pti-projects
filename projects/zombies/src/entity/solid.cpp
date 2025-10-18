#include "pti/pti.h"
#include "solid.h"
#include "actor.h"
#include "registry.h"
#include "../gamestate.h"

#include <cmath>

void Solid::Physics(void) {
	MoveX(speed.x);
	MoveY(speed.y);
}

void Solid::MoveX(float amount) {
	remainder.x += amount;
	int move = std::round(remainder.x);
	if (move != 0) {
		auto actors = GetEntitiesOfType<Actor>();
		int dx = _pti_sign(move);
		while (move != 0) {
			for (auto *actor : actors) {
				if (Overlaps(actor, {dx, 0})) {
					actor->MoveX(dx, &Actor::Squish);
				} else if (actor->IsRidding(this)) {
					actor->MoveX(dx, nullptr);
				}
			}
			position.x += dx;
			move -= dx;
		}
		remainder.x = 0;
	}
}

void Solid::MoveY(float amount) {
	remainder.y += amount;
	int move = std::round(remainder.y);
	if (move != 0) {
		auto actors = GetEntitiesOfType<Actor>();
		int dy = _pti_sign(move);
		while (move != 0) {
			for (auto *actor : actors) {
				if (Overlaps(actor, {0, dy})) {
					actor->MoveY(dy, &Actor::Squish);
				} else if (actor->IsRidding(this)) {
					actor->MoveY(dy, nullptr);
				}
			}
			position.y += dy;
			move -= dy;
		}
		remainder.y = 0;
	}
}