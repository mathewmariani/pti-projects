#pragma once

#include <string>

typedef struct pti_palette_t pti_palette_t;
typedef struct pti_flags_t pti_flags_t;
typedef struct pti_tilemap_t pti_tilemap_t;
typedef struct pti_tileset_t pti_tileset_t;
typedef struct pti_bitmap_t pti_bitmap_t;
typedef struct pti_sound_t pti_sound_t;

namespace batteries {

	extern void init();
	extern void reload();

	extern pti_palette_t *palette(const std::string &path);
	extern pti_bitmap_t *sprite(const std::string &path);
	extern pti_flags_t *flags(const std::string &path);
	extern pti_tileset_t *tileset(const std::string &path);
	extern pti_tilemap_t *tilemap(const std::string &path);
	extern pti_sound_t *sound_wav(const std::string &path);

	extern pti_sound_t create_sine_tone(float frequency, float amplitude, float duration_seconds, int sample_rate, int num_channels);
}// namespace batteries