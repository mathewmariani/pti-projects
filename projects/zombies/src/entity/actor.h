#pragma once

#include "batteries/entity.h"

struct Actor : EntityBase {
	using MoveFunc = void (Actor::*)(void);

	void Physics(void);

	void MoveX(float amount, MoveFunc func);
	void MoveY(float amount, MoveFunc func);

	bool CanWiggle(void);
	void Squish(void);
	virtual void HaltX(void);
	virtual void HaltY(void);

	bool IsRiding(const EntityBase *base) const;
	bool IsGrounded(void) const;

	bool CollidesWithSolids(const CoordXY<int> &dir) const;

	virtual bool CanBeMoved(void) const { return true; }

protected:
	bool grounded;
};