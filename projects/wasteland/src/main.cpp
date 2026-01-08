/* engine */
#include "pti/pti.h"

#include "bank.h"
#include "gamestate.h"
#include "batteries/helper.h"
#include "batteries/juice.h"

#include <math.h>
#include <string>

#if defined(PTI_DEBUG)
#include "imgui/imgui.h"
bool show_overlays = false;
#endif

static void init(void) {
	GameStateInit();
}

static void cleanup(void) {
	/* ... */
}

static void frame(void) {
	auto &gameState = GetGameState();
	if (pti_down(PTI_DBG)) {
		GetGameState().Reset();
		GameStateInit();
		return;
	}

	GetGameState().Tick();
	DoShake();
}

#if defined(PTI_DEBUG)
void debug(void) {
	ImGui::Begin("Dungeon", NULL, ImGuiWindowFlags_AlwaysAutoResize);

	const auto &gameState = GetGameState();

	if (ImGui::CollapsingHeader("core", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("fps: %.1f", ImGui::GetIO().Framerate);

		const size_t used_bytes = 0;    //(size_t) (_pti.cart.it - _pti.cart.begin);
		const size_t capacity_bytes = 0;//(size_t) (_pti.cart.cap - _pti.cart.begin);
		const float used_kb = used_bytes / 1024.0f;
		const float capacity_kb = capacity_bytes / 1024.0f;

		ImGui::Text("usage: %.2f KB / %.2f KB (%.2f%%)\n", used_kb, capacity_kb, (used_kb / capacity_kb) * 100.0);
	}

	if (ImGui::CollapsingHeader("game", ImGuiTreeNodeFlags_DefaultOpen)) {
		int cam_x, cam_y;
		pti_get_camera(&cam_x, &cam_y);
		ImGui::Text("camera: (%d, %d)", cam_x, cam_y);

		auto entities = GetEntitiesOfType<EntityBase>();
		ImGui::Text("entities: (%d)", (int) entities.size());

		ImGui::Checkbox("overlay", &show_overlays);

		if (ImGui::Button("reload level")) {
			std::printf("reload level\n");
		}
	}

	ImGui::End();
}
#endif

constexpr int kScreenWidth = 256;
constexpr int kScreenHeight = 256;

pti_desc pti_main(int argc, char *argv[]) {

	return (pti_desc) {
			.init_cb = init,
			.frame_cb = frame,
			.cleanup_cb = cleanup,
#if defined(PTI_DEBUG)
			.debug_cb = debug,
#endif
			.memory_size = _pti_kilobytes(256),
			.width = kScreenWidth,
			.height = kScreenHeight,
	};
}