#pragma once

#include "base.h"
#include "../world/coordinate.h"

#include <utility>

constexpr uint8_t kMaxEntities = 255;

template<typename T, typename... Args>
EntityBase *CreateEntity(Args &&...args);

void RemoveEntity(EntityBase *entity);
void ResetAllEntities();
void RenderAllEntities();
bool CheckCollisionsWith(const EntityBase *self, EntityBase *&other, const CoordXY<int> &dir);