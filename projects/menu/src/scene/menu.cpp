#include "menu.h"
#include "../gamestate.h"

#include "pti/pti.h"

#include <array>
#include <variant>
#include <type_traits>
#include <cstdio>

template<int Min, int Max, int Default = (Min + Max) / 2>
struct BoundedNumber {
    static_assert(Min <= Max, "Min must be <= Max");
    static_assert(Default >= Min && Default <= Max, "Default must be within bounds");

    int value;
    constexpr BoundedNumber(int v = Default): value(clamp(v)) {}
    constexpr BoundedNumber& operator++() {
        value = clamp(value + 1);
        return *this;
    }
    constexpr BoundedNumber operator++(int) {
        BoundedNumber old = *this;
        ++(*this);
        return old;
    }
    constexpr BoundedNumber& operator--() {
        value = clamp(value - 1);
        return *this;
    }
    constexpr BoundedNumber operator--(int) {
        BoundedNumber old = *this;
        --(*this);
        return old;
    }
    constexpr operator int() const { return value; }

private:
    static constexpr int clamp(int v) {
        return (v < Min) ? Min : (v > Max) ? Max : v;
    }
};

using SfxVolume = BoundedNumber<0, 5, 3>;
using GammaRange = BoundedNumber<0, 100, 50>;

using Variant = std::variant<std::monostate, SfxVolume, GammaRange, bool>;

struct MenuItem {
	const char *name;
	Variant value;
};

std::array<MenuItem, 6> items{{
		{"SFX", SfxVolume()},
		{"MUSIC", SfxVolume()},
		{"FULLSCREEN", false},
		{"FILTER", false},
		{"GAMMA", GammaRange()},
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
		auto& item = items[current_index];

		std::visit([&](auto& v) {
			using T = std::decay_t<decltype(v)>;
			if constexpr (std::is_same_v<T, std::monostate>) {
				// nothing to do
			}
			else if constexpr (std::is_same_v<T, bool>) {
				if (pti_pressed(PTI_LEFT) || pti_pressed(PTI_RIGHT)) {
					v = !v;
					pti_sfx(&tone, 0, 0);
				}
			}
			else {
				// Any BoundedNumber<...>
				if (pti_pressed(PTI_LEFT)) {
					--v;
					pti_sfx(&tone, 0, 0);
				}
				else if (pti_pressed(PTI_RIGHT)) {
					++v;
					pti_sfx(&tone, 0, 0);
				}
			}
		}, item.value);		
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
		std::visit([&](auto& v) {
			using T = std::decay_t<decltype(v)>;
		
			if constexpr (std::is_same_v<T, std::monostate>) {
				// no value
			}
			else if constexpr (std::is_same_v<T, bool>) {
				pti_print(v ? "ON" : "OFF", 100, y);
			}
			else {
				// Any BoundedNumber<...>
				char buffer[32];
				snprintf(buffer, sizeof(buffer), "%d", (int)v);
				pti_print(buffer, 100, y);
			}
		}, item.value);
	}
}