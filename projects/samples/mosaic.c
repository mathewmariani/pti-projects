// stdlib
#include <math.h>
#include <stdlib.h>

// engine
#include "pti/pti.h"
#include "palettes.h"
#define pal sweetie16

static int light[] = {2, 3, 5, 7, 9, 11};
static int dark[] = {1, 4, 6, 8, 10, 12};

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

	for (int i = 0; i < 500; i++) {
		// position:
		int x = rand() % 128;
		int y = rand() % 128;

		// offset:
		float xx = x + (cosf(t * 5) + 0.5f) * 2;
		float yy = y + (sinf(t * 5) + 0.5f) * 2;

		// squares:
		int w = 16;
		int h = 16;

		// color:
		int s = 6;// num squares
		int n = 6;// num colors

		// diamond:
		int i = ((int) (floor(xx / w + yy / h) - floor(xx / w - yy / h)) % s) % n;
		// square:
		// int i = ((int)(floor(xx / w) - floor(yy / h)) % s) % n;

		unsigned int l = light[i];
		unsigned int d = dark[i];
		uint64_t c = ((uint64_t) pal[l] << 32) | (uint64_t) pal[d];

		// local c = light[i]
		pti_pset(x, y, c);
		// pti.circ(x,y,1,c)
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