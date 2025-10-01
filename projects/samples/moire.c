// stdlib
#include <math.h>
#include <stdlib.h>

// engine
#include "pti/pti.h"
#include "palettes.h"
#define pal sweetie16

static void init(void) {
	// gfx state
	pti_dither(0x5a5a);
	// pti_dither(0xeae0);
	pti_clip(0, 0, 128, 128);
}

static void cleanup(void) {}

float t = 0.0f;
static void frame(void) {
	t += (1 / 30.0f);

	// lissajous curves:
	float cx1 = 64 - sinf(t / 3) * (128 * 0.5f);
	float cy1 = 64 - cosf(t / 2) * (128 * 0.5f);
	float cx2 = 64 + sinf(t / 3) * (128 * 0.5f);
	float cy2 = 64 + cosf(t / 2) * (128 * 0.5f);

	int x, y;
	for (y = 0; y < 128; y++) {
		float dy1 = (y - cy1) * (y - cy1);
		float dy2 = (y - cy2) * (y - cy2);
		for (x = 0; x < 128; x++) {
			float dx1 = (x - cx1) * (x - cx1);
			float dx2 = (x - cx2) * (x - cx2);
			int c = (((int) sqrtf(dx1 + dy1) ^ (int) sqrtf(dx2 + dy2)) >> 4);
			if ((c & 1) == 0) {
				if (rand() % 5 == 1) {
					pti_pset(x, y, ((uint64_t) pal[1] << 32) | (uint64_t) pal[2]);
				}
			} else {
				if (rand() % 5 == 1) {
					pti_pset(x, y, ((uint64_t) pal[3] << 32) | (uint64_t) pal[4]);
				}
			}
		}
	}
}

pti_desc pti_main(int argc, char *argv[]) {
	return (pti_desc) {
			.init_cb = init,
			.frame_cb = frame,
			.cleanup_cb = cleanup,
			.memory_size = _pti_kilobytes(128),
			.window =
					(pti_window) {
							.width = 128,
							.height = 128,
							.flags = PTI_SCALE3X,
					},
	};
}
