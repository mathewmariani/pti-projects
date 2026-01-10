#pragma once

#include "batteries/actor.h"

struct Crawler : Actor {
	enum class State {
		Idle,
		Smash,
		Move,
	};

	Crawler(const CoordXY<int>& position);

	void Update() override;
	void Render() override;

	bool IsIdle() const {
		return state == State::Idle;
	}

private:
	CoordXY<int> RandomDirection() const;

	bool IsWalkable(const CoordXY<int> &dir) const;
	void DestroyTile();
	void ChangeDirection();
	void DrunkWalk();
	void HandleMovement();

private:
	State state;
	int idle_timer = 0;
	int move_distance = 0;

	int animation_timer = 0;
	int animation_index = 0;
	int animation_frame = 0;
};