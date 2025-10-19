#include "zombie.h"
#include "../registry.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

#include <random>

constexpr float kZombieMaxSpeed = 0.4f;
constexpr float kZombieAcceleration = 1.80f;
constexpr float kZombieFriction = 15.0f;
constexpr float kZombieKnockback = 2.0f;
constexpr float kZombieRoamingChance = 0.2f;
constexpr float kZombieRoamingMinTime = 1.0f;
constexpr float kZombieRoamingMaxTime = 3.0f;

constexpr float kZombieAlertMinTime = 1.0f;
constexpr float kZombieAlertMaxTime = 3.0f;

constexpr int kZombieOffsetX = 6;
constexpr int kZombieOffsetY = 12;
constexpr int kZombieHitboxOffsetX = -2;
constexpr int kZombieHitboxOffsetY = -2;
constexpr int kZombieHitboxWidth = 6;
constexpr int kZombieHitboxHeight = 6;
constexpr int kZombieFrameCount = 8;
constexpr int kZombieFrameMod = 2;
constexpr int kZombieHealth = 3;

void Zombie::Create() {
	auto *zombie = (Zombie *) CreateEntity<Zombie>();
	if (zombie == nullptr) {
		return;
	}

	std::random_device rd; // seed
	std::mt19937 gen(rd());// Mersenne Twister engine
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	CoordXY<int> position{dist(gen) * (float) EN_ROOM_WIDTH, dist(gen) * (float) EN_ROOM_HEIGHT};

	zombie->SetLocation(position);
}

Zombie::Zombie() {
	bx = kZombieHitboxOffsetX;
	by = kZombieHitboxOffsetY;
	bw = kZombieHitboxWidth;
	bh = kZombieHitboxHeight;
	state = ZombieState::Normal;
	health = kZombieHealth;
}

std::vector<Zombie *> Zombie::GetNeighborhood() {
	std::vector<Zombie *> neighborhood{};
	for (auto *other : GetEntitiesOfType<Zombie>()) {
		if (this == other) {
			continue;
		}
		auto dist2 = position.SqrDistance(other->position);
		if (dist2 <= (32.0f * 32.0f)) {
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
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	auto player = GetGameState().player;
	auto dist_to_player = position.SqrDistance(player->position);

	// --- Detect player ---
	if (dist_to_player < (32.0f * 32.0f)) {
		state = ZombieState::Alerted;
		if (alertTimer <= 0.0f) {
			alertTimer = kZombieAlertMinTime + dist(gen) * kZombieAlertMaxTime;
		}
	} else {
		if (state == ZombieState::Alerted) {
			if (alertTimer <= 0.0f) {
				state = ZombieState::Normal;
			}
		}
	}

	// --- Check neighborhood for alerted zombies ---
	bool nearbyAlert = false;
	for (auto *other : GetNeighborhood()) {
		if (other->state == ZombieState::Alerted) {
			nearbyAlert = true;
			moveDir = other->moveDir;
			break;
		}
	}

	if (state == ZombieState::Alerted) {
		moveTimer -= PTI_DELTA;
		moveDir = position.DirectionTo(player->position);
	} else if (state == ZombieState::Normal) {
		moveTimer -= PTI_DELTA;
		if (moveTimer <= 0.0f) {
			if (dist(gen) < kZombieRoamingChance) {
				moveDir = {dist(gen) * 2.0f - 1.0f, dist(gen) * 2.0f - 1.0f};
				float len = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
				if (len > 0.0f) {
					moveDir.x /= len;
					moveDir.y /= len;
				}
			} else {
				moveDir = CoordXY<float>::Zero;
			}
			moveTimer = kZombieRoamingMinTime + dist(gen) * kZombieRoamingMaxTime;
		}
	}

	// --- Movement/friction ---
	if (moveDir.x != 0.0f || moveDir.y != 0.0f) {
		_pti_appr(speed.x, kZombieMaxSpeed * moveDir.x, kZombieAcceleration * PTI_DELTA);
		_pti_appr(speed.y, kZombieMaxSpeed * moveDir.y, kZombieAcceleration * PTI_DELTA);
	} else {
		_pti_appr(speed.x, 0.0f, kZombieFriction * PTI_DELTA);
		_pti_appr(speed.y, 0.0f, kZombieFriction * PTI_DELTA);
	}

	// --- Avoid neighbors ---
	CoordXY<float> avoid = CoordXY<float>::Zero;
	CoordXY<float> heading = CoordXY<float>::Zero;
	for (auto *other : GetNeighborhood()) {
		auto offset = other->position - position;
		auto d2 = other->position.SqrDistance(position);
		heading = heading + moveDir;
		if (d2 < (16.0f * 16.0f)) {
			avoid = avoid - (offset * (1.0f / d2));
		}
	}

	CoordXY<float> force = moveDir + avoid * 0.5f;// scale avoidance influence

	float len = std::sqrt(force.x * force.x + force.y * force.y);
	if (len > 0.0f) {
		force.x /= len;
		force.y /= len;
	}

	_pti_appr(speed.x, kZombieMaxSpeed * force.x, kZombieAcceleration * PTI_DELTA);
	_pti_appr(speed.y, kZombieMaxSpeed * force.y, kZombieAcceleration * PTI_DELTA);
	// --- Player collision ---
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
	pti_circ(position.x, position.y, 32, 0xffff0000);
	pti_circ(position.x, position.y, 16, 0xffff0000);
}