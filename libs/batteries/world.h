#pragma once

#include <functional>

struct Actor;
struct Solid;

struct IWorld {
	virtual ~IWorld() = default;
	virtual void ForEachActor(const std::function<void(Actor &)> &fn) = 0;
	virtual void ForEachSolid(const std::function<void(Solid &)> &fn) = 0;
};
