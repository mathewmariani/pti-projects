#include "menu.h"
#include "../gamestate.h"

#include "pti/pti.h"

#include <cstdlib>

using Variant = std::variant<std::monostate, int, bool>;

struct MenuItem {
	const char *name;
	Variant value;
};

std::array<MenuItem, 6> items{{
		{"SFX", 5},
		{"MUSIC", 1},
		{"FULLSCREEN", false},
		{"FILTER", false},
		{"GAMMA", 50},
		{"POP-UPS", false},
}};

pti_audio_t tone;
int current_index = 0;

void MenuScene::Init(void) {
	constexpr float sample_rate = 44100.0f;
	constexpr float beep_duration = 0.1f;
	const int num_frames = (int) (sample_rate * beep_duration);

	current_index = 0;
	tone.num_frames = num_frames;
	tone.num_channels = 1;
	tone.samples = (float *) malloc(tone.num_frames * sizeof(float));

	float frequency = 300.0f;
	float amplitude = 0.15f;
	float phase = 0.0f;

	for (int i = 0; i < tone.num_frames; i++) {
		tone.samples[i] = sinf(phase) * amplitude;
		phase += 2.0f * 3.14159265f * frequency / sample_rate;
		if (phase >= 2.0f * 3.14159265f) phase -= 2.0f * 3.14159265f;
	}
}

void MenuScene::Update(void) {
	if (pti_pressed(PTI_DOWN)) {
		pti_sfx(&tone, 0, 0);
		current_index++;
	} else if (pti_pressed(PTI_UP)) {
		pti_sfx(&tone, 0, 0);
		current_index--;
	} else {
		auto &item = items[current_index];
		if (std::holds_alternative<int>(item.value)) {
			int &i = std::get<int>(item.value);
			if (pti_pressed(PTI_LEFT)) {
				pti_sfx(&tone, 0, 0);
				i--;
			} else if (pti_pressed(PTI_RIGHT)) {
				pti_sfx(&tone, 0, 0);
				i++;
			}
		} else if (std::holds_alternative<bool>(item.value)) {
			if (pti_pressed(PTI_LEFT) || pti_pressed(PTI_RIGHT)) {
				bool &b = std::get<bool>(item.value);
				b = !b;
				pti_sfx(&tone, 0, 0);
			}
		}
	}

	current_index = _pti_clamp(current_index, 0, (int) items.size() - 1);
}

void MenuScene::Render(void) {
	pti_cls(0xffef7d57);

	pti_print("OPTIONS", 12, 8);

	for (auto i = 0; i < items.size(); ++i) {
		auto y = 16 + ((i + 1) * 10);
		auto &item = items[i];

		// print cursor:
		if (current_index == i) {
			pti_print(">", 12, y);
		}

		// print name:
		pti_print(item.name, 24, y);

		// print value
		if (std::holds_alternative<int>(item.value)) {
			int v = std::get<int>(item.value);
			char buffer[32];
			snprintf(buffer, sizeof(buffer), "%d", v);// convert int to string
			pti_print(buffer, 100, y);
		} else if (std::holds_alternative<bool>(item.value)) {
			bool b = std::get<bool>(item.value);
			pti_print(b ? "ON" : "OFF", 100, y);
		} else {
			// no value
		}
	}
}