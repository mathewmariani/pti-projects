#include "pti/pti.h"

#include "batteries/actor.h"
#include "batteries/solid.h"
#include "batteries/assets.h"

#include <math.h>

#include "game.h"
#include "../gamestate.h"

#include "../entity/actor/crawler.h"

bool flag = false;

// palette stored in cache
static uint32_t sweetie16[] = {
	0xff2c1c1a,
	0xff5d275d,
	0xff533eb1,
	0xff577def,
	0xff75cdff,
	0xff70f0a7,
	0xff64b738,
	0xff797125,
	0xff6f3629,
	0xffc95d3b,
	0xfff6a641,
	0xfff7ef73,
	0xfff4f4f4,
	0xffc2b094,
	0xff866c56,
	0xff573c33,
};
static pti_palette_t palette = {
	.count = 16,
	.colors = &sweetie16[0],
};

// tilemap stored in cache
constexpr int kTilemapSize = 64;
constexpr int kTilemapCount = kTilemapSize*kTilemapSize;
static uint8_t tiles[kTilemapCount];
static pti_tilemap_t tilemap = {
	.width = kTilemapSize,
	.height = kTilemapSize,
	.tiles = &tiles[0],
};

// tileset stored in cache
constexpr int kTilesetSize = 5;
static uint8_t indices[] = {
	1, 1, 2, 2, 3, 3, 4, 4, 5, 5,
	1, 1, 2, 2, 3, 3, 4, 4, 5, 5,
};
static pti_tileset_t tileset = {
	.count = kTilesetSize,
	.width = kTilesetSize * 10,
	.height = 2,
	.tile_w = 2,
	.tile_h = 2,
	.pixels = &indices[0],
};

void GameScene::Init(void) {
	{ // FIXME: ugly hack.
		if (!flag) {
			pti_set_palette(&palette);
			pti_set_tilemap(&tilemap);
			pti_set_tileset(&tileset);
			flag = true;
		}

		for (int i = 0; i < kTilemapCount; i++) {
			tiles[i] = 1;
		}
	}

	Reset();

	if (auto *e = CreateEntity<Crawler>(); e) {
		e->SetLocation({64, 64});
	}
}

void GameScene::Update(void) {
	// return;
	auto crawlers = GetEntitiesOfType<Crawler>();
	int count = 0;
	for (int y = 0; y < kTilemapSize; y++) {
		for (int x = 0; x < kTilemapSize; x++) {
			count += pti_mget(x, y) == 2 ? 1 : 0;
		}
	}
	if (((float)count / (64.0f * 64.0f)) > 0.15f) {
		for (auto *c : crawlers) {
			c->Destroy();
		}
		return;
	}
	for (auto *c : crawlers) {
		if (crawlers.size() > 1) {
			if ((rand() % 100) > 95) {
				c->Destroy();
				continue;
			}
		}
		if (crawlers.size() < 10) {
			if ((rand() % 100) > 5) {
				auto *e = CreateEntity<Crawler>();
				e->position = c->position;
				e->direction = c->direction;		
			}
		}
		c->Update();
	}
}
void GameScene::Render(void) {
	pti_cls(15);
	pti_map(0, 0);
	RenderEntitiesOfType<EntityBase>();

	// int x = rand() % 16;
	// int y = rand() % 16;
	// pti_mset(x, y, 0);
}