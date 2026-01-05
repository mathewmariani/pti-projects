#include "zombie.h"
#include "../../gamestate.h"
#include "../../bank.h"
#include "pti/pti.h"

#include <random>

constexpr float kZombieMaxSpeed = 0.4f;
constexpr float kZombieAcceleration = 1.80f;
constexpr float kZombieFriction = 15.0f;
constexpr float kZombieKnockback = 0.85f;
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

	CoordXY<int> position{(int) (dist(gen) * 528.0f), (int) (dist(gen) * 384.0f)};

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
		if (dist2 <= (32.0f * 32.0f) && CanSee(other)) {
			neighborhood.push_back(other);
		}
	}
	return neighborhood;
}

void Zombie::Hurt(const CoordXY<int> &direction) {
	speed = direction * kZombieKnockback;
	health -= 1;

	state = ZombieState::Alerted;
	alertTimer = kZombieAlertMaxTime;

	if (health <= 0) {
		// Coin::Create(position);
		Effect::Create(position, Effect::Type::Collect);
		Destroy();
	}
}

constexpr int kTileSize = 8;

bool Zombie::CanSee(const Actor *e) const {
	int x0 = static_cast<int>(position.x / kTileSize);
	int y0 = static_cast<int>(position.y / kTileSize);
	int x1 = static_cast<int>(e->position.x / kTileSize);
	int y1 = static_cast<int>(e->position.y / kTileSize);

	int dx = std::abs(x1 - x0);
	int dy = std::abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (true) {
		auto tile = pti_mget(x0, y0);
		if (pti_fget(tile) != 0) {
			return false;
		}
		if (x0 == x1 && y0 == y1) {
			break;
		}
		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}

	return true;
}

void Zombie::Update() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	auto player = GetGameState().player;
	auto dist_to_player = position.SqrDistance(player->position);

	if (dist_to_player < (32.0f * 32.0f) && CanSee(player)) {
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

	bool nearbyAlert = false;
	for (auto *other : GetNeighborhood()) {
		if (other->state == ZombieState::Alerted) {
			nearbyAlert = true;
			moveDir = other->moveDir;
			break;
		}
	}

	if (state == ZombieState::Alerted) {
		alertTimer -= PTI_DELTA;
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

	if (moveDir.x != 0.0f || moveDir.y != 0.0f) {
		_pti_appr(speed.x, kZombieMaxSpeed * moveDir.x, kZombieAcceleration * PTI_DELTA);
		_pti_appr(speed.y, kZombieMaxSpeed * moveDir.y, kZombieAcceleration * PTI_DELTA);
	} else {
		_pti_appr(speed.x, 0.0f, kZombieFriction * PTI_DELTA);
		_pti_appr(speed.y, 0.0f, kZombieFriction * PTI_DELTA);
	}

	auto avoid = CoordXY<float>::Zero;
	auto heading = CoordXY<float>::Zero;
	for (auto *other : GetNeighborhood()) {
		auto offset = other->position - position;
		auto d2 = other->position.SqrDistance(position);
		heading = heading + moveDir;
		if (d2 < (16.0f * 16.0f)) {
			avoid = avoid - (offset * (1.0f / d2));
		}
	}

	auto force = moveDir + avoid;
	_pti_appr(speed.x, kZombieMaxSpeed * force.x, kZombieAcceleration * PTI_DELTA);
	_pti_appr(speed.y, kZombieMaxSpeed * force.y, kZombieAcceleration * PTI_DELTA);

	auto collision = false;
	for (auto *player : GetCollisions<Player>(this, direction)) {
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
	// pti_circ(position.x, position.y, 32, 0xffff0000);
	// pti_circ(position.x, position.y, 16, 0xffff0000);
}