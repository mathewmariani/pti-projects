#pragma once

#include "base.h"

struct Actor : EntityBase {
	static constexpr auto cEntityType = EntityType::Actor;

	using MoveFunc = void (Actor::*)(void);

	void Physics(void);

	void MoveX(float amount, MoveFunc func);
	void MoveY(float amount, MoveFunc func);

	void Squish(void);
	void HaltX(void);
	void HaltY(void);

	bool IsRidding(const EntityBase *base) const;
	bool IsGrounded(void) const;

	bool CollidesWithSolids(const CoordXY<int> &dir) const;
};