#include "pti/pti.h"

#include "tilemap.h"
#include "../bank.h"

void TilemapRender() {
	pti_map(tilemap, tileset, 0, 0);
}