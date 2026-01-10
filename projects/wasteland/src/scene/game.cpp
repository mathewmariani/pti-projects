#include "pti/pti.h"

#include "batteries/actor.h"
#include "batteries/solid.h"
#include "batteries/assets.h"

#include <math.h>

#include "../bank.h"

#include "game.h"
#include "../gamestate.h"

#include "../entity/actor/crawler.h"

bool flag = false;

// palette stored in cache
// static uint32_t sweetie16[] = {
// 	0xff2c1c1a,
// 	0xff5d275d,
// 	0xff533eb1,
// 	0xff577def,
// 	0xff75cdff,
// 	0xff70f0a7,
// 	0xff64b738,
// 	0xff797125,
// 	0xff6f3629,
// 	0xffc95d3b,
// 	0xfff6a641,
// 	0xfff7ef73,
// 	0xfff4f4f4,
// 	0xffc2b094,
// 	0xff866c56,
// 	0xff573c33,
// };
// static pti_palette_t palette = {
// 	.count = 16,
// 	.colors = &sweetie16[0],
// };

// // tilemap stored in cache
// constexpr int kTilemapSize = 64;
// constexpr int kTilemapCount = kTilemapSize*kTilemapSize;
// static uint8_t tiles[kTilemapCount];
// static pti_tilemap_t tilemap = {
// 	.width = kTilemapSize,
// 	.height = kTilemapSize,
// 	.tiles = &tiles[0],
// };

// tileset stored in cache
// constexpr int kTilesetSize = 5;
// static uint8_t indices[] = {
// 	1, 1, 2, 2, 3, 3, 4, 4, 5, 5,
// 	1, 1, 2, 2, 3, 3, 4, 4, 5, 5,
// };
// static pti_tileset_t tileset = {
// 	.count = kTilesetSize,
// 	.width = kTilesetSize * 10,
// 	.height = 2,
// 	.tile_w = 2,
// 	.tile_h = 2,
// 	.pixels = &indices[0],
// };

constexpr int kTilemapSize = 32;

void GameScene::Init(void) {
	{ // FIXME: ugly hack.
		if (!flag) {
			// load external assets
			batteries::init();
			palette = batteries::palette("assets/tilemap3.ase");
			tilemap = batteries::tilemap("assets/tilemap3.ase");
			tileset = batteries::tileset("assets/tilemap3.ase");
			batteries::reload();

			// load the bank data into pti
			pti_set_palette(palette);
			pti_set_tilemap(tilemap);
			pti_set_tileset(tileset);
			flag = true;
		}
	}

	Reset();

	for (int i = 0; i < kTilemapSize; i++) {
		for (int j = 0; j < kTilemapSize; j++) {
			pti_mset(i, j, 7);
		}
	}

	CreateEntity<Crawler>(CoordXY<int>{256, 256});
}

void GameScene::Update(void) {
	// return;
	auto crawlers = GetEntitiesOfType<Crawler>();
	int count = 0;
	for (int y = 0; y < kTilemapSize; y++) {
		for (int x = 0; x < kTilemapSize; x++) {
			count += pti_mget(x, y) == 0 ? 1 : 0;
		}
	}
	if (((float)count / (30.0f * 30.0f)) > 0.50f) {
		for (auto *c : crawlers) {
			c->Destroy();
		}
		return;
	}
	for (auto *c : crawlers) {
		c->Update();
		if (c->IsIdle()) {
			if (crawlers.size() > 5) {
				if ((rand() % 100) > 95) {
					c->Destroy();
					continue;
				}
			}
			if (crawlers.size() < 10) {
				if ((rand() % 100) <= 5) {
					CreateEntity<Crawler>(c->position);
				}
			}
		}
		
	}
}

static const uint8_t AUTOTILE_LUT[16] = {
    /* 0000 */ 13,  // isolated

	/* 0001 */ 1,  // down
    /* 0010 */ 16,  // left
    /* 0011 */ 4,  // left + down
    /* 0100 */ 9,  // top
	/* 0101 */ 5,  // top + down
    /* 0110 */ 12,  // top + left
    /* 0111 */ 8,  // top + left + down

    /* 1000 */ 14,  // right
    /* 1001 */ 2,  // right + down
    /* 1010 */ 15, // right + left
    /* 1011 */ 3, // right + left + down
    /* 1100 */ 10, // right + top

    /* 1101 */ 6, // right + top + down
    
	/* 1110 */ 11, // left + right + down
    /* 1111 */ 7, // surrounded
};
enum VonNeumannMask : uint8_t
{
    VN_UP    = 1 << 0, // 0001
    VN_LEFT  = 1 << 1, // 0010
    VN_DOWN  = 1 << 2, // 0100
    VN_RIGHT = 1 << 3, // 1000
};

uint8_t get_vonneumann_mask(int x, int y)
{
    uint8_t mask = 0;

    const auto& neigh = CoordXY<int>::VonNewmanNeighborhood();

    // Expected order: { Up, Left, Down, Right }
    for (size_t i = 0; i < neigh.size(); ++i)
    {
        const auto& d = neigh[i];

        // pti_mget != 0 → 1, else 0, shifted into the correct bit
        mask |= (pti_mget(x + d.x, y + d.y) != 0) << i;
    }

    return mask;
}


void GameScene::Render(void) {
	// if ((rand() % 100) > 50) {
	// int x = rand() % 16;
	// pti_cls(x);
	// }
	for (int i = 0; i < kTilemapSize; i++) {
		for (int j = 0; j < kTilemapSize; j++) {
			if (pti_mget(i, j) == 0) {
				continue;
			}
			uint8_t mask = get_vonneumann_mask(i, j);
			pti_mset(i, j, AUTOTILE_LUT[mask]);
		}
	}
	pti_cls(0);
	pti_map(0, 0);
	RenderEntitiesOfType<EntityBase>();
}