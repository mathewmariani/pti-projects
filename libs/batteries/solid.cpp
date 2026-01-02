#include "pti/pti.h"
#include "solid.h"
#include "actor.h"
#include "gamestate.h"

#include <unordered_set>

inline void CollectRidingActors(const Solid *solid, const CoordXY<int> dir, std::unordered_set<Actor *> &out) {
	World()->ForEachActor([&](Actor &actor) {
		if (!actor.CanBeMoved()) {
			return;
		}
		if (actor.IsRiding(solid) || solid->Overlaps(&actor, dir)) {
			out.insert(&actor);
		}
	});
}

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

	remainder.x -= move;
	int dx = _pti_sign(move);

	std::unordered_set<Actor *> riding;
	CollectRidingActors(this, {dx, 0}, riding);

	while (move != 0) {
		position.x += dx;
		move -= dx;
		for (auto *actor : riding) {
			actor->MoveX(dx, &Actor::Squish);
		}
	}
}

void Solid::MoveY(float amount) {
	remainder.y += amount;
	int move = static_cast<int>(remainder.y);
	if (move == 0) {
		return;
	}

	remainder.y -= move;
	int dy = _pti_sign(move);

	std::unordered_set<Actor *> riding;
	CollectRidingActors(this, {0, dy}, riding);

	while (move != 0) {
		position.y += dy;
		move -= dy;
		for (auto *actor : riding) {
			actor->MoveY(dy, &Actor::Squish);
		}
	}
}