#pragma once

#include <unordered_set>

#include "actor.h"
#include "solid.h"

template<typename EntityManagerT>
struct PhysicsSystem {
	EntityManagerT &entities;

	void ActorMoveX(Actor &actor, float amount, Actor::MoveFunc func = nullptr) {
		actor.remainder.x += amount;
		int move = std::round(actor.remainder.x);
		if (move != 0) {
			actor.direction.x = _pti_sign(move);
			actor.remainder.x -= move;
			const int dx = actor.direction.x;
			const CoordXY<int> dir{dx, 0};
			while (move != 0) {
				// moving up slope:
				if (actor.PlaceMeeting({dx, 0}) && !(actor.PlaceMeeting({dx, -1}))) {
					actor.position.y -= 1;
				}
				// moving down slope:
				if (!(actor.PlaceMeeting({dx, 0})) && !(actor.PlaceMeeting({dx, 1})) && actor.PlaceMeeting({dx, 2})) {
					actor.position.y += 1;
				}
				// always last:
				if (!actor.PlaceMeeting(dir) && !ActorCollidesWithSolids(actor, dir)) {
					actor.position.x += dx;
					move -= dx;
				} else {
					if (func) {
						(actor.*func)();
					}
					break;
				}
			}
		}
	}

	void ActorMoveY(Actor &actor, float amount, Actor::MoveFunc func = nullptr) {
		actor.remainder.y += amount;
		int move = std::round(actor.remainder.y);
		if (move != 0) {
			actor.direction.y = _pti_sign(move);
			actor.remainder.y -= move;
			const int dy = actor.direction.y;
			const CoordXY<int> dir{0, dy};
			while (move != 0) {
				if (!actor.PlaceMeeting(dir) && !ActorCollidesWithSolids(actor, dir)) {
					actor.position.y += dy;
					move -= dy;
				} else {
					auto squished = false;
					if (dy < 0) {
						// moving up:
						squished = !ActorCanWiggle(actor);
					} else if (dy > 0) {
						// moving down:
						squished = (actor.PlaceMeeting({0, -1}) || ActorCollidesWithSolids(actor, {0, -1}));
					}
					if (squished && func) {
						(actor.*func)();
					}
					break;
				}
			}
		}
	}

	void SolidMoveX(Solid &solid, float amount) {
		solid.remainder.x += amount;
		int move = static_cast<int>(solid.remainder.x);
		if (move == 0) {
			return;
		}

		solid.remainder.x -= move;
		int dx = _pti_sign(move);

		std::unordered_set<Actor *> riding;
		CollectRidingActors(solid, {dx, 0}, riding);

		while (move != 0) {
			solid.position.x += dx;
			move -= dx;
			for (auto *actor : riding) {
				ActorMoveX(*actor, dx, &Actor::Squish);
			}
		}
	}

	void SolidMoveY(Solid &solid, float amount) {
		solid.remainder.y += amount;
		int move = static_cast<int>(solid.remainder.y);
		if (move == 0) {
			return;
		}

		solid.remainder.y -= move;
		int dy = _pti_sign(move);

		std::unordered_set<Actor *> riding;
		CollectRidingActors(solid, {0, dy}, riding);

		while (move != 0) {
			solid.position.y += dy;
			move -= dy;
			for (auto *actor : riding) {
				ActorMoveY(*actor, dy, &Actor::Squish);
			}
		}
	}

	bool ActorCollidesWithSolids(const Actor &actor, const CoordXY<int> &dir) {
		bool collided = false;
		entities.template ForEach<Solid>([&](Solid *solid) {
			if (actor.Overlaps(solid, dir)) {
				collided = true;
			}
		});
		return collided;
	}

	bool ActorCanWiggle(Actor &actor) {
		for (auto i = 0; i < 4; ++i) {
			for (auto j : {-1, 1}) {
				const CoordXY<int> dir{i * j, -1};
				if (!actor.PlaceMeeting(dir) && !ActorCollidesWithSolids(actor, dir)) {
					actor.position.x += (i * j);
					return true;
				}
			}
		}
		return false;
	}

	bool IsActorGrounded(Actor &actor) {
		const auto dir = CoordXY<int>::Up;
		return actor.PlaceMeeting(dir) || ActorCollidesWithSolids(actor, dir);
	}

private:
	void CollectRidingActors(const Solid &solid, const CoordXY<int> dir, std::unordered_set<Actor *> &out) {
		entities.template ForEach<Actor>([&](Actor *actor) {
			if (actor->IsRiding(&solid) ||
				solid.Overlaps(actor, dir)) {
				out.insert(actor);
			}
		});
	}
};
