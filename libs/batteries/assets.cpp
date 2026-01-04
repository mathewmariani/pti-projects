#include "assets.h"

#include <unordered_map>
#include <math.h>

#include "pti/pti.h"
#include "cute/cute_aseprite.h"
#include "stb/stb_vorbis.c"

namespace batteries {

	static std::unordered_map<std::string, pti_bitmap_t> _sprite_cache;
	static std::unordered_map<std::string, pti_tileset_t> _tileset_cache;
	static std::unordered_map<std::string, pti_tilemap_t> _tilemap_cache;

	pti_bank_t bank;

	void init() {
		pti_bank_init(&bank, _pti_kilobytes(256));
	}

	void reload() {
		pti_load_bank(&bank);
	}

	pti_bitmap_t __create_bitmap(const std::string &path) {
		auto *ase = cute_aseprite_load_from_file(path.c_str(), NULL);

		/* allocate bitmap data */
		auto size = ase->w * ase->h * sizeof(ase_color_t);
		auto *data = (char *) pti_alloc(&bank, size * ase->frame_count);

		/* initialize */
		pti_bitmap_t bitmap;
		bitmap.frames = (int32_t) ase->frame_count;
		bitmap.width = (int16_t) ase->w;
		bitmap.height = (int16_t) ase->h;
		bitmap.pixels = data;

		for (int i = 0; i < ase->frame_count; ++i, data += size) {
			auto *frame = ase->frames + i;
			memcpy(data, frame->pixels, size);
		}

		/* release cute resources. */
		cute_aseprite_free(ase);

		return bitmap;
	}

	pti_tileset_t __create_tileset(const std::string &path) {
		ase_t *ase = cute_aseprite_load_from_file(path.c_str(), NULL);
		ase_tileset_t ase_tileset = ase->tileset;

		int tile_w = ase_tileset.tile_w;
		int tile_h = ase_tileset.tile_h;
		int count = ase_tileset.tile_count;
		const ase_color_t *src_pixels = (ase_color_t *) ase_tileset.pixels;

		int tiles_per_row = (int) ceilf(sqrtf((float) count));
		int atlas_w = tiles_per_row * tile_w;
		int atlas_h = ((count + tiles_per_row - 1) / tiles_per_row) * tile_h;

		ase_color_t *atlas_pixels = (ase_color_t *) pti_alloc(&bank, atlas_w * atlas_h * sizeof(ase_color_t));
		memset(atlas_pixels, 0, atlas_w * atlas_h * sizeof(ase_color_t));

		for (int i = 0; i < count; ++i) {
			int tile_x = (i % tiles_per_row) * tile_w;
			int tile_y = (i / tiles_per_row) * tile_h;

			for (int y = 0; y < tile_h; ++y) {
				const ase_color_t *src_row = &src_pixels[i * tile_w * tile_h + y * tile_w];
				ase_color_t *dst_row = &atlas_pixels[(tile_y + y) * atlas_w + tile_x];
				memcpy(dst_row, src_row, tile_w * sizeof(ase_color_t));
			}
		}

		/* release cute resources. */
		cute_aseprite_free(ase);

		return (pti_tileset_t) {
				.count = (uint32_t) count,
				.width = (uint16_t) atlas_w,
				.height = (uint16_t) atlas_h,
				.tile_w = (uint16_t) tile_w,
				.tile_h = (uint16_t) tile_h,
				.pixels = atlas_pixels,
		};
	}

	pti_tilemap_t __create_tilemap(const std::string &path) {
		ase_t *ase = cute_aseprite_load_from_file(path.c_str(), NULL);

		pti_tilemap_t tilemap;
		for (int i = 0; i < ase->frame_count; ++i) {
			ase_frame_t *frame = ase->frames + i;
			for (int j = 0; j < frame->cel_count; ++j) {
				ase_cel_t *cel = frame->cels + j;
				if (cel->is_tilemap) {
					const size_t size = cel->w * cel->h * sizeof(int);
					tilemap.width = (int16_t) cel->w;
					tilemap.height = (int16_t) cel->h;
					tilemap.tiles = (int *) pti_alloc(&bank, size);
					memcpy(tilemap.tiles, cel->tiles, size);
				}
			}
		}

		/* release cute resources. */
		cute_aseprite_free(ase);

		return tilemap;
	}

	pti_bitmap_t *sprite(const std::string &path) {
		if (_sprite_cache.find(path) == _sprite_cache.end()) {
			_sprite_cache.emplace(std::make_pair(path, __create_bitmap(path)));
		}
		return &_sprite_cache[path];
	}

	pti_tileset_t *tileset(const std::string &path) {
		if (_tileset_cache.find(path) == _tileset_cache.end()) {
			_tileset_cache.emplace(std::make_pair(path, __create_tileset(path)));
		}
		return &_tileset_cache[path];
	}

	pti_tilemap_t *tilemap(const std::string &path) {
		if (_tilemap_cache.find(path) == _tilemap_cache.end()) {
			_tilemap_cache.emplace(std::make_pair(path, __create_tilemap(path)));
		}
		return &_tilemap_cache[path];
	}

	pti_sound_t create_sine_tone(float frequency, float amplitude, float duration_seconds, int sample_rate, int num_channels) {
		pti_sound_t tone = {0};
	
		tone.samples_count = (int) (duration_seconds * sample_rate);
		tone.channels = num_channels;
	
		tone.samples = (int16_t *) pti_alloc(&bank, sizeof(int16_t) * tone.samples_count * num_channels);
	
		if (!tone.samples) {
			tone.samples_count = 0;
			tone.channels = 0;
			return tone;// allocation failed
		}
	
		if (amplitude > 1.0f) amplitude = 1.0f;
		if (amplitude < 0.0f) amplitude = 0.0f;
	
		float phase = 0.0f;
		float phase_inc = 2.0f * 3.14159265f * frequency / (float) sample_rate;
	
		for (int i = 0; i < tone.samples_count; i++) {
			float value = sinf(phase) * amplitude;
	
			// clamp
			if (value > 1.0f) value = 1.0f;
			if (value < -1.0f) value = -1.0f;
	
			int16_t sample = (int16_t) (value * 32767.0f);
	
			for (int ch = 0; ch < num_channels; ch++) {
				tone.samples[i * num_channels + ch] = sample;
			}
	
			phase += phase_inc;
			if (phase >= 2.0f * 3.14159265f) phase -= 2.0f * 3.14159265f;
		}
	
		return tone;
	}
}// namespace batteries