#include "juice.h"

#include "pti/pti.h"

#include <cmath>  // for std::abs
#include <cstdlib>// for rand(), RAND_MAX

constexpr float kShakeOffset = 8.0f;
constexpr float kShakeLowerBound = kShakeOffset * 0.5f;

namespace {
	static float shakeTimer = 0.0f;
}

inline float rnd(float n) {
	return static_cast<float>(rand()) / RAND_MAX * n;
}

void Shake(void) {
	shakeTimer = 1.0f;
}

void DoShake(void) {
	// generate random offsets between -16 and +16
	float shakex = kShakeLowerBound - rnd(kShakeOffset);
	float shakey = kShakeLowerBound - rnd(kShakeOffset);

	// apply shake strength
	shakex *= shakeTimer;
	shakey *= shakeTimer;

	// apply camera offset
	pti_camera(shakex, shakey);

	// fade out the shake
	shakeTimer *= 0.95f;
	if (shakeTimer < 0.05f) {
		shakeTimer = 0.0f;
	}
}