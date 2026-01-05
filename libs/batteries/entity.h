#pragma once

#include "coordinate.h"

#include <algorithm>
#include <stdint.h>

using EntityId = uint32_t;

enum class Collisions : uint8_t {
	None = 0x00,
	Solid = 0x01,
	Semi = 0x02,
	SpikeT = 0x03,
	SpikeR = 0x04,
	SpikeB = 0x05,
	SpikeL = 0x06,
	SlopeRB = 0x07,
	SlopeRT = 0x08,
	SlopeLT = 0x09,
	SlopeLB = 0x0A,
};

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
	bool PlaceMeeting(const CoordXY<int> &dir, const int index) const;

	void Destroy(void);

private:
	template<typename Predicate>
	bool CheckTiles(const CoordXY<int> &dir, Predicate &&pred) const;

	bool PlaceMeetingForTile(int i, int j, const CoordXY<int> &dir, uint8_t flags) const;
};