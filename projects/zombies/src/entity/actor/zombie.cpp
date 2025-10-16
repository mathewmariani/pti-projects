#include "zombie.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

void Zombie::Hurt(const CoordXY<int> &direction) {
	sx = direction.x * kZombieKnockback;
	sy = direction.y * kZombieKnockback;
	health -= 1;

	if (health <= 0) {
		Coin::Create({x, y});
		Effect::Create({x, y}, Effect::Type::Collect);
		RemoveEntity(this);
	}
}

void Zombie::Update() {
	// move towards player'
	auto player = GetGameState().player;
	CoordXY<int> a{x, y};
	CoordXY<int> b{player->x, player->y};
	CoordXY<float> dir = a.DirectionTo(b);
	_pti_appr(sx, dir.x, kZombieFriction * PTI_DELTA);
	_pti_appr(sy, dir.y, kZombieFriction * PTI_DELTA);


	auto collision = false;
	for (auto *player : GetCollisions<Player>(*this, direction)) {
		CoordXY<int> a{x, y};
		CoordXY<int> b{player->x, player->y};
		CoordXY<float> dir = a.DirectionTo(b);
		player->Hurt(dir);
		collision = true;
	}
}

void Zombie::Render() {
	auto frame = static_cast<int>(timer * kZombieFrameCount) % kZombieFrameMod;
	if (sx == 0 && sy == 0) {
		frame = 0;
	}

	pti_spr(bitmap_zombie, frame, x - kZombieOffsetX, y - kZombieOffsetY, false, false);
}