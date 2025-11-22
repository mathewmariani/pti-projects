#include "menu.h"
#include "../gamestate.h"

#include "pti/pti.h"

#include <cstdlib>

struct MenuItem {
	const char *name;
};

constexpr std::array<MenuItem, 3> items{{
		{"Play"},
		{"Settings"},
		{"Exit"},
}};

int current_index = 0;

void MenuScene::Init(void) {
	current_index = 0;
}

void MenuScene::Update(void) {
	if (pti_pressed(PTI_DOWN)) {
		current_index++;
	} else if (pti_pressed(PTI_UP)) {
		current_index--;
	} else if (pti_pressed(PTI_A)) {
		switch (current_index) {
		case 0:
			GetGameState().SwitchScenes(SceneType::Game);
			break;
		case 1:
			/* code */
			break;
		case 2:
			std::exit(0);
			break;
		}
	}

	current_index = _pti_clamp(current_index, 0, (int) items.size() - 1);
}

void MenuScene::Render(void) {
	pti_cls(0xffef7d57);

	pti_print("Rock Toss", 12, 8);

	for (auto i = 0; i < items.size(); ++i) {
		auto y = 16 + ((i + 1) * 10);
		pti_print(items[i].name, 24, y);
		if (current_index == i) {
			pti_print(">", 12, y);
		}
	}
}