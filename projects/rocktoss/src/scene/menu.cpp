#include "menu.h"

#include "pti/pti.h"

void MenuScene::Init(void) {
	/* body */
}

void MenuScene::Update(void) {
	/* body */
}

void MenuScene::Render(void) {
	pti_cls(0xffef7d57);
	pti_print("Play", 12, 8);
	pti_print("Settings", 12, 18);
	pti_print("Exit", 12, 28);
}