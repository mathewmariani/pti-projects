#include "pti/pti.h"

#include "batteries/actor.h"
#include "batteries/solid.h"
#include "batteries/assets.h"

#include <array>
#include <math.h>

#include "../bank.h"

#include "game.h"
#include "../gamestate.h"

#include "../entity/actor/player.h"
#include "../entity/actor/crawler.h"

bool flag = false;

constexpr int kTilemapSize = 32;

void GameScene::Init(void) {
	{ // FIXME: ugly hack.
		if (!flag) {
			// load external assets
			batteries::init();
			flags = batteries::flags("assets/flags.bin");
			palette = batteries::palette("assets/tilemap3.ase");
			tilemap = batteries::tilemap("assets/tilemap3.ase");
			tileset = batteries::tileset("assets/tilemap3.ase");
			player_bitmap = batteries::sprite("assets/link.ase");
			batteries::reload();

			// load the bank data into pti
			pti_set_flags(flags);
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
	CreateEntity<Player>(CoordXY<int>{256, 256});
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
	if (((float)count / (30.0f * 30.0f)) > 0.35f) {
		for (auto *c : crawlers) {
			c->Destroy();
		}
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

	UpdateEntitiesOfType<Player>();
}

static constexpr std::array<std::array<uint8_t, 4>, 16> AUTOTILE_LUT = {{
    /* 0000 */ {{13, 13, 13, 13}},  // isolated
    /* 0001 */ {{1, 1, 1, 1}},   // up
    /* 0010 */ {{16, 16, 16, 16}},  // left
    /* 0011 */ {{4, 4, 4, 4}},   // up + left
    /* 0100 */ {{9, 9, 9, 9}},   // down
    /* 0101 */ {{5, 5, 5, 5}},   // up + down
    /* 0110 */ {{12, 12, 12, 12}},  // left + down
    /* 0111 */ {{8, 8, 8, 8}},   // up + left + down
    /* 1000 */ {{14, 14, 14, 14}},  // right
    /* 1001 */ {{2, 2, 2, 2}},   // up + right
    /* 1010 */ {{15, 15, 15, 15}},  // left + right
    /* 1011 */ {{3, 3, 3, 3}},  // up + left + right
    /* 1100 */ {{10, 10, 10, 10}},  // down + right
    /* 1101 */ {{6, 6, 6, 6}},   // up + down + right
    /* 1110 */ {{11, 11, 11, 11}},   // left + down + right
    /* 1111 */ {{7, 7, 7, 7}},  // surrounded
}};

uint8_t GetVonNeumannMask(int x, int y)
{
	const auto& neigh = CoordXY<int>::VonNewmanNeighborhood();
    
    // Expected order: { top, Left, Down, Right }
	uint8_t mask = 0;
    for (size_t i = 0; i < neigh.size(); ++i)
    {
        const auto& d = neigh[i];
        mask |= (pti_mget(x + d.x, y + d.y) != 0) << i;
    }

    return mask;
}

inline uint8_t TileVariation(int x, int y) noexcept
{
    uint32_t h = static_cast<uint32_t>(x) * 73856093u ^ static_cast<uint32_t>(y) * 19349663u;
    return static_cast<uint8_t>(h & 3); // % 4
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

			auto mask = GetVonNeumannMask(i, j);
            auto v = TileVariation(i, j);
            pti_mset(i, j, AUTOTILE_LUT[mask][v]);
		}
	}
	pti_cls(0);
	pti_map(0, 0);
	RenderEntitiesOfType<EntityBase>();
}