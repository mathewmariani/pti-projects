#include "pti/pti.h"
#include "solid.h"
#include "actor.h"
#include "../gamestate.h"

#include <unordered_set>

void Solid::Physics(void) {
	MoveX(speed.x);
	MoveY(speed.y);
}

void Solid::MoveX(float amount) {
	remainder.x += amount;
	int move = static_cast<int>(remainder.x);
	if (move == 0) {
		return;
	}

	const auto actors = GetEntitiesOfType<Actor>();

	remainder.x -= move;
	int dx = _pti_sign(move);

	std::unordered_set<Actor *> riding;
	for (auto *actor : actors) {
		if (actor->IsRiding(this)) {
			riding.insert(actor);
		}
	}

	while (move != 0) {
		position.x += dx;
		move -= dx;

		for (auto *actor : actors) {
			if (riding.contains(actor)) {
				actor->MoveX(dx, &Actor::Squish);
			}
		}
	}
}

void Solid::MoveY(float amount) {
	remainder.y += amount;
	int move = static_cast<int>(remainder.y);
	if (move == 0) {
		return;
	}

	auto actors = GetEntitiesOfType<Actor>();

	remainder.y -= move;
	int dy = _pti_sign(move);

	std::unordered_set<Actor *> riding;
	for (auto *actor : actors) {
		if (actor->IsRiding(this)) {
			riding.insert(actor);
		}
	}

	while (move != 0) {
		position.y += dy;
		move -= dy;

		for (auto *actor : actors) {
			if (riding.contains(actor)) {
				actor->MoveY(dy, &Actor::Squish);
			}
		}
	}
}