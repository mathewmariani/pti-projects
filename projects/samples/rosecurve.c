// stdlib
#include <math.h>
#include <stdlib.h>

// engine
#include "pti/pti.h"
#include "palettes.h"
#define pal sweetie16

static void init(void) {
	// gfx state
	// pti_dither(0x5a5a);
	pti_dither(0xeae0);
	pti_clip(0, 0, 128, 128);
}

static void cleanup(void) {}

float time = 0.0f;
static void frame(void) {
	// framebuffer dimensions
	const int width = 128;
	const int height = 128;
	const int center_x = width / 2;
	const int center_y = height / 2;

	// rose curve parameters
	const int petal_radius = 32;
	const int petal_count = 5;
	const int color_scale = 16;

	// time-dependent effects
	float rotation_angle = cosf(time) + time;
	float growth_bias = sinf(time) * sinf(time) * time;
	float palette_offset = 0.2f;

	for (int py = 0; py < height; py++) {
		for (int px = 0; px < width; px++) {
			// position relative to the center (cartesian coordinates)
			int rel_x = center_x - px;
			int rel_y = center_y - py;

			// convert to polar coordinates
			float angle_rad = atan2f((float) rel_y, (float) rel_x);           // θ
			float pixel_dist = sqrtf((float) (rel_x * rel_x + rel_y * rel_y));// distance from center

			// ideal rose-curve radius at this angle: r(θ) = a * cos(nθ)
			float rose_radius = petal_radius * cosf(petal_count * angle_rad + rotation_angle);

			// color calculation: combine geometry
			uint8_t color_index = (uint8_t) ((rose_radius + pixel_dist - growth_bias) / color_scale);
			uint8_t low_index = (uint8_t) (color_index + palette_offset) % 16;
			uint8_t high_index = (uint8_t) (color_index - palette_offset) % 16;

			pti_pset(px, py, ((unsigned long) pal[low_index] << 32) | pal[high_index]);
		}
	}

	time += PTI_DELTA;
}


pti_desc pti_main(int argc, char *argv[]) {
	return (pti_desc) {
			.init_cb = init,
			.frame_cb = frame,
			.cleanup_cb = cleanup,
			.memory_size = _pti_kilobytes(128),
			.window =
					(pti_window) {
							.name = "pti - rosecurve",
							.width = 128,
							.height = 128,
							.flags = PTI_SCALE3X,
					},
	};
}