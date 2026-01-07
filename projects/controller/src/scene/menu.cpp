#include "pti/pti.h"

// batteries
#include "batteries/assets.h"

#include "menu.h"
#include "../bank.h"
#include "../gamestate.h"

bool flag = false;

void MenuScene::Init(void) {
	{ // FIXME: ugly hack.
		if (!flag) {
			batteries::init();
			palette = batteries::palette("assets/palette.hex");
			bitmap_a = batteries::sprite("assets/btn_a.ase");
			bitmap_b = batteries::sprite("assets/btn_b.ase");
			bitmap_x = batteries::sprite("assets/btn_x.ase");
			bitmap_y = batteries::sprite("assets/btn_y.ase");
			bitmap_left = batteries::sprite("assets/btn_left.ase");
			bitmap_right = batteries::sprite("assets/btn_right.ase");
			bitmap_up = batteries::sprite("assets/btn_up.ase");
			bitmap_down = batteries::sprite("assets/btn_down.ase");
			bitmap_start = batteries::sprite("assets/btn_start.ase");
			bitmap_select = batteries::sprite("assets/btn_select.ase");
			bitmap_rshoulder = batteries::sprite("assets/btn_rshoulder.ase");
			bitmap_lshoulder = batteries::sprite("assets/btn_lshoulder.ase");
			batteries::reload();

			pti_set_palette(palette);

			flag = true;
		}

		// reload loads the specific bank into pti
		batteries::reload();
	}
}
void MenuScene::Update(void) {}

#define DRAW_BTN(btn, bitmap, x, y)                          \
	do {                                                     \
		state = (pti_down(btn) || pti_pressed(btn)) ? 1 : 0; \
		pti_spr(bitmap, state, x, y, false, false);          \
	} while (0)

void MenuScene::Render(void) {
	pti_cls(15);

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