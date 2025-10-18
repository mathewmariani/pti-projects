#pragma once

#include "../actor.h"

constexpr float kZombieMaxSpeed = 0.4f;
constexpr float kZombieAcceleration = 1.80f;
constexpr float kZombieFriction = 15.0f;
constexpr float kZombieKnockback = 2.0f;
constexpr float kZombieRoamingChance = 0.2f;
constexpr float kZombieRoamingMinTime = 1.0f;
constexpr float kZombieRoamingMaxTime = 3.0f;

constexpr float kZombieAlertMinTime = 1.0f;
constexpr float kZombieAlertMaxTime = 3.0f;

constexpr int kZombieOffsetX = 4;
constexpr int kZombieOffsetY = 8;
constexpr int kZombieWidth = 8;
constexpr int kZombieHeight = 8;
constexpr int kZombieFrameCount = 8;
constexpr int kZombieFrameMod = 2;
constexpr int kZombieHealth = 3;

enum class ZombieState : uint8_t {
	Normal,
	Alerted,
};

struct Zombie : Actor {
	Zombie() {
		bx = 0;
		by = 0;
		bw = kZombieWidth;
		bh = kZombieHeight;
		state = ZombieState::Normal;
		health = kZombieHealth;
	}

	static void Create();

	std::vector<Zombie *> GetNeighborhood();
	void Hurt(const CoordXY<int> &direction);

	void Update() override;
	void Render() override;

private:
	ZombieState state;
	int health;
	float moveTimer;
	float alertTimer;
	CoordXY<float> moveDir;
};