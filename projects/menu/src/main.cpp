/* engine */
#include "pti/pti.h"

/* batteries */
#include "batteries/assets.h"
#include "batteries/helper.h"
#include "batteries/juice.h"

#include "bank.h"
#include "gamestate.h"

#include <math.h>
#include <string>

#if defined(PTI_DEBUG)
#include "imgui/imgui.h"
bool show_overlays = false;
#endif

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

static void load(void) {
	GameStateInit();
	batteries::reload();
}

static void init(void) {
	batteries::init();
	bitmap_font = batteries::sprite("assets/font.ase");

	pti_set_font(bitmap_font);

	load();

	/* graphics state: */
	pti_clip(0, 0, EN_ROOM_WIDTH, EN_ROOM_HEIGHT);
}

static void cleanup(void) {
	/* ... */
}

static void frame(void) {
	auto &gameState = GetGameState();
	if (pti_down(PTI_DBG)) {
		GetGameState().Reset();
		load();
		return;
	}

	GetGameState().Tick();
	DoShake();

#if defined(PTI_DEBUG)
	if (show_overlays) {
		for (auto *e : GetEntitiesOfType<EntityBase>()) {
			pti_rect(e->position.x + e->bx, e->position.y + e->by, e->bw - 1, e->bh - 1, 0xff00ff00);
		}
	}
#endif
}

#if defined(PTI_DEBUG)
void debug(void) {
	ImGui::Begin("Towerfall Physics", NULL, ImGuiWindowFlags_AlwaysAutoResize);

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

	// if (ImGui::CollapsingHeader("entities", ImGuiTreeNodeFlags_DefaultOpen)) {
	// 	auto players = GetEntitiesOfType<Player>();
	// 	if (players.size() > 0) {
	// 		auto *player = players[0];
	// 		ImGui::Text("position: (%d, %d)", player->position.x, player->position.y);
	// 		ImGui::Text("speed: (%.2f, %.2f)", player->speed.x, player->speed.y);
	// 		ImGui::Text("grounded: (%s)", player->IsGrounded() ? "true" : "false");

	// 		if (ImGui::Button("hurt")) {
	// 			std::printf("hurt\n");
	// 		}
	// 		if (ImGui::Button("kill")) {
	// 			std::printf("kill\n");
	// 		}
	// 	} else {
	// 		ImGui::Text("player is dead\n");
	// 	}
	// }

	ImGui::End();
}
#endif

pti_desc pti_main(int argc, char *argv[]) {

	return (pti_desc) {
			.init_cb = init,
			.frame_cb = frame,
			.cleanup_cb = cleanup,
#if defined(PTI_DEBUG)
			.debug_cb = debug,
#endif
			.memory_size = _pti_kilobytes(256), /* 256KB */
			.width = 176,
			.height = 128,
	};
}