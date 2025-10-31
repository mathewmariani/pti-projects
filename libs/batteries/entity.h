#pragma once

#include "coordinate.h"

#include <algorithm>
#include <stdint.h>

using EntityId = uint32_t;

struct BoundingBox {
	int top, left, width, height;
	BoundingBox() : top(0), left(0), width(0), height(0) {}
	BoundingBox(int x, int y, int w, int h) : top(x), left(y), width(w), height(h) {}
	BoundingBox(const BoundingBox &) = default;
	BoundingBox(BoundingBox &&) = default;
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