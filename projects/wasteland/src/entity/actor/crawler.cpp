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
constexpr int kTilemapSize = 32;

Crawler::Crawler(const CoordXY<int>& pos) {
	position = pos;
	direction = RandomDirection();
}

CoordXY<int> Crawler::RandomDirection() const {
    for (int tries = 0; tries < 4; ++tries) {
		auto i = rand() % 4;
        CoordXY<int> d = CoordXY<int>::VonNewmanNeighborhood()[i];
        if (IsWalkable(d)) {
            return d;
		}
    }

    return CoordXY<int>::Zero; // nowhere to go
}

bool Crawler::IsWalkable(const CoordXY<int> &dir) const {
	CoordXY<int> newPos = position + (dir * kTileSize);
	int x = newPos.x / kTileSize;
	int y = newPos.y / kTileSize;
	if (x < 1 || x >= kTilemapSize - 1 || y < 1 || y >= kTilemapSize - 1) {
		return false;
	}
	return true;
}

void Crawler::DrunkWalk() {
	if (rand() % 2 == 0) {
		direction = RandomDirection();
	} else if (!IsWalkable(direction)) {
		direction = RandomDirection();
	}
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
	pti_mset(x, y, 0);
}

void Crawler::Update() {
	switch (state) {
		case State::Idle: {
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
				idle_timer = 2;
			}
		} break;
	}
}

void Crawler::Render() {
	pti_rect(position.x, position.y, kTileSize, kTileSize, 2);
}