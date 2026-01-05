#include "pti/pti.h"

#include "player.h"

#include <algorithm>

#include "../../bank.h"      /* bitmap_player */
#include "../../gamestate.h" /* PTI_DELTA */

#include "batteries/coordinate.h"

constexpr int kSpriteOffsetX = 8;
constexpr int kSpriteOffsetY = 8;
constexpr int kWalkDistance = 8;
constexpr int kAnimationTimer = 4;

// should be 8...
// should be defined elsewhere
constexpr int kTileSize = 16;
constexpr int kTilemapSize = 128;

Player::Player() {
	bx = -8;
	by = 0;
	bw = 16;
	bh = 8;
}

bool Player::Walkable(const CoordXY<int> &distance) const {
	const auto top = std::max(0, (position.y + by + distance.y) / kTileSize);
	const auto left = std::max(0, (position.x + bx + distance.x) / kTileSize);
	const auto bottom = std::min(kTilemapSize - 1, (position.y + by + bh + distance.y - 1) / kTileSize);
	const auto right = std::min(kTilemapSize - 1, (position.x + bx + bw + distance.x - 1) / kTileSize);

	for (int j = top; j <= bottom; ++j) {
		for (int i = left; i <= right; ++i) {
			int flags = pti_fget(i, j);
			switch (flags) {
				case 6:
				case 16:
					return false;
			}
		}
	}
	return true;
}

void Player::WaitForInput() {
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

void Player::HandleMovement() {
	position = position + direction;

	animation_timer++;
	if (animation_timer >= kAnimationTimer) {
		animation_timer = 0;
		animation_frame++;
		animation_frame %= 2;
	}

	move_distance--;
	if (move_distance <= 0) {
		state = State::Idle;
	}
}

void Player::Update() {
	switch (state) {
		case State::Idle: {
			WaitForInput();
		} break;
		case State::Move: {
			HandleMovement();
			auto &gameState = GetGameState();
			gameState.currentRoom = position / 128;

		} break;
	}
}

void Player::Render() {
	auto x = position.x - kSpriteOffsetX;
	auto y = position.y - kSpriteOffsetY;
	auto i = animation_index + animation_frame;

	pti_spr(bitmap_player, i, x, y, false, false);
}