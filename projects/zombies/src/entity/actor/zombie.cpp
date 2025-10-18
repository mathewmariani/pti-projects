#include "zombie.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

std::vector<Zombie *> Zombie::GetNeighborhood() {
	std::vector<Zombie *> neighborhood{};
	for (auto *other : GetEntitiesOfType<Zombie>()) {
		if (this == other) {
			continue;
		}
		auto dist2 = position.SqrDistance(other->position);
		if (dist2 <= 5.0f) {
			neighborhood.push_back(other);
		}
	}
	return neighborhood;
}

void Zombie::Hurt(const CoordXY<int> &direction) {
	speed = direction * kZombieKnockback;
	health -= 1;

	if (health <= 0) {
		Coin::Create(position);
		Effect::Create(position, Effect::Type::Collect);
		RemoveEntity(this);
	}
}

void Zombie::Update() {
	// move towards player'
	auto player = GetGameState().player;
	auto dir = position.DirectionTo(player->position);
	_pti_appr(speed.x, dir.x, kZombieFriction * PTI_DELTA);
	_pti_appr(speed.y, dir.y, kZombieFriction * PTI_DELTA);

	auto collision = false;
	for (auto *player : GetCollisions<Player>(*this, direction)) {
		auto dir = position.DirectionTo(player->position);
		player->Hurt(dir);
		collision = true;
	}
}

void Zombie::Render() {
	auto frame = static_cast<int>(timer * kZombieFrameCount) % kZombieFrameMod;
	if (speed == CoordXY<float>::Zero) {
		frame = 0;
	}

	pti_spr(bitmap_zombie, frame, position.x - kZombieOffsetX, position.y - kZombieOffsetY, false, false);
}