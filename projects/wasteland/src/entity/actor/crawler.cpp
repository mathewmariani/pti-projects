#include "pti/pti.h"

#include "crawler.h"

#include <algorithm>

#include "../../gamestate.h" /* PTI_DELTA */

#include "batteries/coordinate.h"

constexpr int kSpriteOffsetX = 8;
constexpr int kSpriteOffsetY = 8;
constexpr int kWalkDistance = 16;
constexpr int kAnimationTimer = 4;

// should be 8...
// should be defined elsewhere
constexpr int kTileSize = 16;
constexpr int kTilemapSize = 256;

Crawler::Crawler() {
	bx = -8;
	by = 0;
	bw = 16;
	bh = 8;
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

		state = State::Smash;
	}
}

void Crawler::DestroyTile() {
	int x = position.x / kTileSize;
	int y = position.y / kTileSize;
	pti_mset(x, y, 21);
}

void Crawler::Update() {
	switch (state) {
		case State::Idle: {
			ChangeDirection();
			DrunkWalk();
		} break;
		case State::Move: {
			HandleMovement();
		} break;
		case State::Smash: {
			idle_timer--;
			if (idle_timer <= 0) {
				DestroyTile();
				state = State::Idle;
				idle_timer = 5;
			}
		} break;
	}
}

void Crawler::Render() {
	pti_rect(position.x, position.y, kTileSize, kTileSize, 2);
}