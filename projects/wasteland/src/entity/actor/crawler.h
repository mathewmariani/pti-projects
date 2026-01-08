#pragma once

#include "batteries/actor.h"

struct Crawler : Actor {
	enum class State {
		Idle,
		Move,
	};

	Crawler();

	void Update() override;
	void Render() override;

public:
	bool Walkable(const CoordXY<int> &distance) const;
	void WaitForInput();
	void DestroyTile();
	void ChangeDirection();
	void DrunkWalk();
	void SpawnCrawler();
	void HandleMovement();

private:
	State state;
	int move_distance = 0;

	int animation_timer = 0;
	int animation_index = 0;
	int animation_frame = 0;
};