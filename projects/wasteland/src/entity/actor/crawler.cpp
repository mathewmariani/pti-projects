#include "pti/pti.h"

#include "crawler.h"

#include <algorithm>

#include "../../gamestate.h" /* PTI_DELTA */

#include "batteries/coordinate.h"

constexpr int kSpriteOffsetX = 8;
constexpr int kSpriteOffsetY = 8;
constexpr int kWalkDistance = 1;
constexpr int kAnimationTimer = 4;

// should be 8...
// should be defined elsewhere
constexpr int kTileSize = 2;
constexpr int kTilemapSize = 64;

Crawler::Crawler() {
	bx = -8;
	by = 0;
	bw = 16;
	bh = 8;
}

bool Crawler::Walkable(const CoordXY<int> &distance) const {
	return true;
}

void Crawler::WaitForInput() {
	auto pos = position;
	auto press = false;
	if (pti_down(PTI_LEFT)) {
		direction = CoordXY<int>::Left;
		animation_index = 4;
		press = true;
	}
	if (pti_down(PTI_RIGHT)) {
		direction = CoordXY<int>::Right;
		animation_index = 6;
		press = true;
	}
	if (pti_down(PTI_UP)) {
		direction = CoordXY<int>::Down;
		animation_index = 2;
		press = true;
	}
	if (pti_down(PTI_DOWN)) {
		direction = CoordXY<int>::Up;
		animation_index = 0;
		press = true;
	}

	auto collision = false;
	if (press) {
		auto next_pos = direction * kWalkDistance;
		if (!Walkable(next_pos)) {
			return;
		}

		state = State::Move;
		move_distance = kWalkDistance;
		HandleMovement();
	} else {
		animation_timer = kAnimationTimer - 1;
		animation_frame = 1;
	}
}

void Crawler::ChangeDirection() {
	if ((rand() % 2) != 0) {
		return;
	}
	int i = rand() % 4;
	switch (i) {
	case 0: direction = CoordXY<int>::Up; break;
	case 1: direction = CoordXY<int>::Down; break;
	case 2: direction = CoordXY<int>::Left; break;
	case 3: direction = CoordXY<int>::Right; break;
	}
}

void Crawler::DrunkWalk() {
	state = State::Move;
	move_distance = kWalkDistance;
	HandleMovement();
}

void Crawler::SpawnCrawler() {
	if ((rand() % 100) > 5) {
		return;
	}
	auto *e = CreateEntity<Crawler>();
	e->position = position;
	e->direction = direction;
}


void Crawler::HandleMovement() {
	position = position + direction;

	animation_timer++;
	if (animation_timer >= kAnimationTimer) {
		animation_timer = 0;
		animation_frame++;
		animation_frame %= 2;
	}

	move_distance--;
	if (move_distance <= 0) {
		int x = position.x / kTileSize;
		int y = position.x / kTileSize;
		pti_mset(x, y, 2);
		state = State::Idle;
	}
}

void Crawler::DestroyTile() {
	int x = (position.x / 16);
	int y = (position.y / 16);
	pti_mset(x, y, 0);
}

void Crawler::Update() {
	switch (state) {
		case State::Idle: {
			// WaitForInput();
			ChangeDirection();
			DrunkWalk();
		} break;
		case State::Move: {
			HandleMovement();
		} break;
	}
}

void Crawler::Render() {
	pti_pset(position.x, position.y, 0);
}