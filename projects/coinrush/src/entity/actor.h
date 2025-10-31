#pragma once

#include "batteries/entity.h"

struct Actor : EntityBase {
	using MoveFunc = void (Actor::*)(void);

	void Physics(void);

	void MoveX(float amount, MoveFunc func);
	void MoveY(float amount, MoveFunc func);

	bool CanWiggle(void);
	void Squish(void);
	void HaltX(void);
	void HaltY(void);

	bool IsRidding(const EntityBase *base) const;
	bool IsGrounded(void) const;

	bool CollidesWithSolids(const CoordXY<int> &dir) const;

protected:
	bool grounded;
};