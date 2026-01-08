#include "assets.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unordered_map>

#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>

#include "pti/pti.h"
#include "cute/cute_aseprite.h"
#include "dr/dr_wav.h"
#include "stb/stb_vorbis.c"

namespace batteries {

	static std::unordered_map<std::string, pti_palette_t> _palette_cache;
	static std::unordered_map<std::string, pti_bitmap_t> _sprite_cache;
	static std::unordered_map<std::string, pti_flags_t> _flags_cache;
	static std::unordered_map<std::string, pti_tileset_t> _tileset_cache;
	static std::unordered_map<std::string, pti_tilemap_t> _tilemap_cache;
	static std::unordered_map<std::string, pti_sound_t> _sound_cache;

	pti_bank_t bank;

	void init() {
		pti_bank_init(&bank, _pti_kilobytes(256));
	}

	void reload() {
		pti_load_bank(&bank);
	}

	pti_palette_t __create_palette(const std::string &path) {
		std::ifstream file(path);
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open palette file");
		}

		std::vector<uint32_t> colors;
		std::string line;

		while (std::getline(file, line)) {
			if (line.empty()) continue;

			if (line.size() != 6) {
				throw std::runtime_error("Invalid hex color: " + line);
			}

			uint32_t rgb = std::stoul(line, nullptr, 16);

			uint8_t r = (rgb >> 16) & 0xFF;
			uint8_t g = (rgb >> 8) & 0xFF;
			uint8_t b = (rgb >> 0) & 0xFF;

			// NOTE: ABGR
			uint32_t argb = (0xFFu << 24) | (b << 16) | (g << 8) | (r << 0);

			colors.push_back(argb);
		}

		pti_palette_t palette;
		palette.count = (uint16_t) colors.size();
		palette.colors = (uint32_t *) pti_alloc(&bank, palette.count * sizeof(uint32_t));

		memcpy(palette.colors, colors.data(), palette.count * sizeof(uint32_t));

		return palette;
	}

	pti_bitmap_t __create_bitmap(const std::string &path) {
		auto *ase = cute_aseprite_load_from_file(path.c_str(), NULL);

		/* allocate bitmap data */
		auto size = ase->w * ase->h * sizeof(uint8_t);
		auto *data = (uint8_t *) pti_alloc(&bank, size * ase->frame_count);

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

	pti_flags_t __create_flags(const std::string &path) {
		pti_flags_t flags = {0, 0};// initialize

		FILE *fp = fopen(path.c_str(), "rb");
		if (!fp) return flags;

		fseek(fp, 0, SEEK_END);
		long filesize = ftell(fp);
		rewind(fp);

		flags.flags = (uint8_t *) pti_alloc(&bank, filesize);
		if (!flags.flags) {
			fclose(fp);
			return flags;
		}

		memset(flags.flags, 0, filesize);// zero memory first
		flags.count = filesize;

		size_t read = fread(flags.flags, 1, flags.count, fp);
		if (read != flags.count) {
			fprintf(stderr, "Warning: read %zu of %zu bytes\n", read, flags.count);
		}

		fclose(fp);
		return flags;
	}


	pti_tileset_t __create_tileset(const std::string &path) {
		ase_t *ase = cute_aseprite_load_from_file(path.c_str(), NULL);
		ase_tileset_t ase_tileset = ase->tileset;

		int tile_w = ase_tileset.tile_w;
		int tile_h = ase_tileset.tile_h;
		int count = ase_tileset.tile_count;
		const uint8_t *src_pixels = (uint8_t *) ase_tileset.pixels;

		int tiles_per_row = (int) ceilf(sqrtf((float) count));
		int atlas_w = tiles_per_row * tile_w;
		int atlas_h = ((count + tiles_per_row - 1) / tiles_per_row) * tile_h;

		uint8_t *atlas_pixels = (uint8_t *) pti_alloc(&bank, atlas_w * atlas_h * sizeof(uint8_t));
		memset(atlas_pixels, 0, atlas_w * atlas_h * sizeof(uint8_t));

		for (int i = 0; i < count; ++i) {
			int tile_x = (i % tiles_per_row) * tile_w;
			int tile_y = (i / tiles_per_row) * tile_h;

			for (int y = 0; y < tile_h; ++y) {
				const uint8_t *src_row = &src_pixels[i * tile_w * tile_h + y * tile_w];
				uint8_t *dst_row = &atlas_pixels[(tile_y + y) * atlas_w + tile_x];
				memcpy(dst_row, src_row, tile_w * sizeof(uint8_t));
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
					tilemap.tiles = (uint8_t *) pti_alloc(&bank, size);
					memcpy(tilemap.tiles, cel->tiles, size);
				}
			}
		}

		/* release cute resources. */
		cute_aseprite_free(ase);

		return tilemap;
	}

	pti_sound_t __create_sound_wav(const std::string &path) {
		unsigned int channels;
		unsigned int rate;
		drwav_uint64 count;
		int16_t *samples = (int16_t *) drwav_open_file_and_read_pcm_frames_s16(path.c_str(), &channels, &rate, &count, NULL);

		const size_t size = sizeof(int16_t) * count * channels;

		pti_sound_t sound = {0};
		sound.channels = channels;
		sound.rate = rate;
		sound.samples_count = count;
		sound.samples = (int16_t *) pti_alloc(&bank, size);
		pti_memcpy(sound.samples, samples, size);

		/* release cute resources. */
		drwav_free(samples, NULL);

		return sound;
	}

	pti_palette_t *palette(const std::string &path) {
		if (_palette_cache.find(path) == _palette_cache.end()) {
			_palette_cache.emplace(std::make_pair(path, __create_palette(path)));
		}
		return &_palette_cache[path];
	}

	pti_bitmap_t *sprite(const std::string &path) {
		if (_sprite_cache.find(path) == _sprite_cache.end()) {
			_sprite_cache.emplace(std::make_pair(path, __create_bitmap(path)));
		}
		return &_sprite_cache[path];
	}

	pti_flags_t *flags(const std::string &path) {
		if (_flags_cache.find(path) == _flags_cache.end()) {
			_flags_cache.emplace(std::make_pair(path, __create_flags(path)));
		}
		return &_flags_cache[path];
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

	pti_sound_t *sound_wav(const std::string &path) {
		if (_sound_cache.find(path) == _sound_cache.end()) {
			_sound_cache.emplace(std::make_pair(path, __create_sound_wav(path)));
		}
		return &_sound_cache[path];
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