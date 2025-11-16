#pragma once

#include "../actor.h"

struct Rock : Actor {
	Rock();

	void Update() override;
	void Render() override;

	void Pickup(const Actor *actor);
	void Throw(const CoordXY<float> &dir);

	void HaltX() override;
	void HaltY() override;

private:
	void HandleHorizontalMovement();
	void HandleVerticalMovement();

	const Actor *owner;
};