#pragma once

#include "batteries/actor.h"

struct Bullet : Actor {
	Bullet();

	void Update() override;
	void Render() override;
};