#pragma once

#include "../actor.h"

struct Rock : Actor {
	Rock();

	void Update() override;
	void Render() override;

	void Pickup(const Actor *actor);
	void Throw(const CoordXY<float> &dir);

private:
	void HandleHorizontalMovement();
	void HandleVerticalMovement();

	const Actor *owner;
};