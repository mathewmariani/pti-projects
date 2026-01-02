#include "pti/pti.h"

#include <memory>
#include <vector>

// batteries
#include "batteries/assets.h"
#include "batteries/helper.h"

// actors
#include "../entity/actor/player.h"

// actors
#include "../entity/solid/platform.h"

#include "../gamestate.h"

#include "game.h"

typedef struct pti_tilemap_t pti_tilemap_t;

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

void GameScene::Init(void) {
	Reset();
	int i, j, t;
	for (i = 0; i < EN_ROOM_COLS; i++) {
		for (j = 0; j < EN_ROOM_ROWS; j++) {
			t = pti_mget(i, j);
			switch (t) {
				case 48: {
					if (auto *e = CreateEntity<Player>(); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
				} break;
				case 51:
					if (auto *e = CreateEntity<Platform>(Platform::Type::Vertical); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
				case 52:
					if (auto *e = CreateEntity<Platform>(Platform::Type::Horizontal); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
			}
		}
	}
}

void GameScene::Update(void) {
	UpdateEntitiesOfType<Solid>();
	UpdateEntitiesOfType<Actor>();
}

void GameScene::Render(void) {
	pti_cls(0xffef7d57);
	pti_map(0, 0);

	RenderEntitiesOfType<EntityBase>();
}