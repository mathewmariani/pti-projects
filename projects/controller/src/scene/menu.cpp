#include "menu.h"
#include "../gamestate.h"
#include "../bank.h"

#include "pti/pti.h"

void MenuScene::Init(void) {}
void MenuScene::Update(void) {}

#define DRAW_BTN(btn, bitmap, x, y)                          \
	do {                                                     \
		state = (pti_down(btn) || pti_pressed(btn)) ? 1 : 0; \
		pti_spr(bitmap, state, x, y, false, false);          \
	} while (0)

void MenuScene::Render(void) {
	pti_cls(0xff575757);
	pti_color(0xffffffff);

	int state, ox, oy;

	ox = 32, oy = 64;
	DRAW_BTN(PTI_A, bitmap_a, ox + 12, oy + 0);
	DRAW_BTN(PTI_B, bitmap_b, ox + 0, oy + 12);
	DRAW_BTN(PTI_X, bitmap_x, ox + 0, oy - 12);
	DRAW_BTN(PTI_Y, bitmap_y, ox - 12, oy + 0);

	ox = 80, oy = 64;
	DRAW_BTN(PTI_LSHOULDER, bitmap_lshoulder, ox - 64, oy - 32);
	DRAW_BTN(PTI_RSHOULDER, bitmap_rshoulder, ox + 64, oy - 32);

	ox = 80, oy = 64;
	DRAW_BTN(PTI_START, bitmap_start, ox + 12, oy + 0);
	DRAW_BTN(PTI_SELECT, bitmap_select, ox - 12, oy + 0);

	ox = 128, oy = 64;
	DRAW_BTN(PTI_RIGHT, bitmap_right, ox + 12, oy + 0);
	DRAW_BTN(PTI_DOWN, bitmap_down, ox + 0, oy + 12);
	DRAW_BTN(PTI_UP, bitmap_up, ox + 0, oy - 12);
	DRAW_BTN(PTI_LEFT, bitmap_left, ox - 12, oy + 0);
}