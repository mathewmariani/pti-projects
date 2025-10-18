#pragma once

#include "../world/coordinate.h"

#include <algorithm>
#include <stdint.h>

// FIXME: this can't be it.
#define PTI_DELTA (1.0 / 30.0)

enum class EntityType : uint8_t {
	Solid,
	Actor,

	// always last.
	Count,
	Null = 255,
};

using EntityId = uint8_t;

struct BoundingBox {
	uint8_t top;
	uint8_t left;
	uint8_t width;
	uint8_t height;
};

struct EntityBase {
	EntityId id;
	CoordXY<int> position;
	CoordXY<float> speed;
	CoordXY<float> remainder;
	uint8_t flags;     // general entity flags
	int bx, by, bw, bh;// bounding box
	float timer;       // general timer, starts at 0 on create

	int32_t start_x;
	int32_t start_y;// position

	CoordXY<int> direction;

	template<typename T>
	bool Is(void) const;

	virtual void Update(void);
	virtual void Render(void);

	void Physics(void);

	void SetLocation(const CoordXY<int> &newLocation);

	bool Overlaps(const EntityBase *other, const CoordXY<int> &dir) const;
	bool PlaceMeeting(const CoordXY<int> &dir) const;
};