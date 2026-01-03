#include "pti/pti.h"

#include <memory>
#include <vector>

// batteries
#include "batteries/assets.h"
#include "batteries/helper.h"

// actors
#include "../entity/actor/bullet.h"
#include "../entity/actor/coin.h"
#include "../entity/actor/effect.h"
#include "../entity/actor/flag.h"
#include "../entity/actor/goomba.h"
#include "../entity/actor/player.h"

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
				case 49:
					Coin::Create({XPOS(i), YPOS(j)});
					pti_mset(i, j, 0);
					break;
				case 50: {
					Goomba::Create({XPOS(i), YPOS(j)});
					pti_mset(i, j, 0);
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
					// shooter
				case 57:
					if (auto *e = CreateEntity<Shooter>(CoordXY<int>::Down); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
				case 58:
					if (auto *e = CreateEntity<Shooter>(CoordXY<int>::Up); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
				case 59:
					if (auto *e = CreateEntity<Shooter>(CoordXY<int>::Right); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;
				case 60:
					if (auto *e = CreateEntity<Shooter>(CoordXY<int>::Left); e) {
						e->SetLocation({XPOS(i), YPOS(j)});
						pti_mset(i, j, 0);
					}
					break;

				case 61:
					if (auto *e = CreateEntity<Flag>(); e) {
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
	pti_cls(0xff575757);
	pti_map(0, 0);

	RenderEntitiesOfType<EntityBase>();
}