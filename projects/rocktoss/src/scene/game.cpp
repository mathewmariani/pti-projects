#include "pti/pti.h"

#include <memory>
#include <vector>

// batteries
#include "batteries/actor.h"
#include "batteries/assets.h"

// actors
#include "../entity/actor/player.h"
#include "../entity/actor/rock.h"

#include "game.h"
#include "../bank.h"
#include "../gamestate.h"

// should be defined elsewhere
constexpr int kTileSize = 8;
constexpr float kDeathResetTimer = 2.0f;

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

bool flag = false;

void GameScene::Init(void) {
	{ // FIXME: ugly hack.
		if (!flag) {
			batteries::init();
			palette = batteries::palette("assets/tilemap.ase");
			flags = batteries::flags("assets/flags.bin");
			tileset = batteries::tileset("assets/tilemap.ase");
			tilemap = batteries::tilemap("assets/tilemap.ase");
			bitmap_player = batteries::sprite("assets/dog.ase");
			bitmap_bullet = batteries::sprite("assets/bullet.ase");
			bitmap_font = batteries::sprite("assets/font.ase");

			pti_set_palette(palette);
			pti_set_flags(flags);
			pti_set_tilemap(tilemap);
			pti_set_tileset(tileset);

			flag = true;
		}

		// reload loads the specific bank into pti
		batteries::reload();
	}

	Reset();
	int i, j, t;
	for (i = 0; i < tilemap->height; i++) {
		for (j = 0; j < tilemap->width; j++) {
			t = pti_mget(i, j);
			switch (t) {
				case 48: {
					if (auto *e = CreateEntity<Player>(); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
				} break;
				case 49: {
					if (auto *e = CreateEntity<Rock>(); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
				} break;
			}
		}
	}
}

void GameScene::Update(void) {
	UpdateEntitiesOfType<Actor>();
}

void GameScene::Render(void) {
	pti_cls(15);
	pti_map(0, 0);

	RenderEntitiesOfType<EntityBase>();
}