#pragma once

#include "entity.h"

struct Actor : EntityBase {
	using MoveFunc = void (Actor::*)(void);

	virtual void Squish(void);
	virtual void HaltX(void);
	virtual void HaltY(void);

	bool IsRiding(const EntityBase *base) const;

	virtual bool CanBeMoved(void) const { return true; }

	bool grounded;
};