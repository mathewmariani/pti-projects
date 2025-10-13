#include "pti/pti.h"
#include "solid.h"
#include "actor.h"
#include "registry.h"
#include "../gamestate.h"

#include <cmath>

void Solid::Physics(void) {
	MoveX(sx);
	MoveY(sy);
}

void Solid::MoveX(float amount) {
	rx += amount;
	int move = std::round(rx);
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
			x += dx;
			move -= dx;
		}
		rx = 0;
	}
}

void Solid::MoveY(float amount) {
	ry += amount;
	int move = std::round(ry);
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
			y += dy;
			move -= dy;
		}
		ry = 0;
	}
}