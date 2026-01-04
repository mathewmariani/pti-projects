#pragma once

#include "batteries/actor.h"

struct Player : Actor {
	enum class State {
		Idle,
		Move,
	};

	Player();

	void Update() override;
	void Render() override;

private:
	bool Walkable(const CoordXY<int> &distance) const;
	void WaitForInput();
	void HandleMovement();

private:
	State state;
	int move_distance = 0;

	int animation_timer = 0;
	int animation_index = 0;
	int animation_frame = 0;
};