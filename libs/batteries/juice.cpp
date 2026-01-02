#include "juice.h"

#include "pti/pti.h"

#include <cmath>  // for std::abs
#include <cstdlib>// for rand(), RAND_MAX

constexpr float kShakeOffset = 8.0f;
constexpr float kShakeLowerBound = kShakeOffset * 0.5f;

namespace {
	static float shakeTimer = 0.0f;
	static int cam_x = 0;
	static int cam_y = 0;
}// namespace

inline float rnd(float n) {
	return static_cast<float>(rand()) / RAND_MAX * n;
}

void Shake() {
	shakeTimer = 1.0f;
	pti_get_camera(&cam_x, &cam_y);
}

void DoShake() {
	if (shakeTimer <= 0.0f) {
		return;
	}

	// generate random offsets between -kShakeLowerBound and +kShakeLowerBound
	float shakex = kShakeLowerBound - rnd(kShakeOffset);
	float shakey = kShakeLowerBound - rnd(kShakeOffset);

	// apply shake strength
	shakex *= shakeTimer;
	shakey *= shakeTimer;

	// apply camera offset
	pti_camera(cam_x + shakex, cam_y + shakey);

	// fade out the shake
	shakeTimer *= 0.95f;
	if (shakeTimer < 0.05f) {
		shakeTimer = 0.0f;
	}
}