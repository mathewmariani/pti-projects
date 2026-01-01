#pragma once

#include "batteries/actor.h"
#include <vector>

enum class ZombieState : uint8_t {
	Normal,
	Alerted,
};

struct Zombie : Actor {
	static void Create();

	Zombie();

	std::vector<Zombie *> GetNeighborhood();
	void Hurt(const CoordXY<int> &direction);

	void Update() override;
	void Render() override;

private:
	bool CanSee(const Actor *actor) const;

private:
	ZombieState state;
	int health;
	float moveTimer;
	float alertTimer;
	CoordXY<float> moveDir;
};