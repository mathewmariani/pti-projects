/* engine */
#include "pti/pti.h"

/* batteries */
#include "batteries/helper.h"
#include "batteries/juice.h"

/* game */
#include "bank.h"
#include "gamestate.h"

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

#if defined(PTI_DEBUG)
	if (show_overlays) {
		for (auto *e : GetEntitiesOfType<EntityBase>()) {
			pti_rect(e->position.x + e->bx, e->position.y + e->by, e->bw - 1, e->bh - 1, 0xff00ff00);
		}
	}
#endif
}

#if defined(PTI_DEBUG)
static const char *SceneTypeToString(SceneType t) {
	switch (t) {
		case SceneType::Game:
			return "Game";
	}
	return "Unknown";
}

static void debug(void) {
	ImGui::Begin("Rock Toss", NULL, ImGuiWindowFlags_AlwaysAutoResize);

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

		static SceneType selected = SceneType::Game;
		if (ImGui::BeginCombo("##SceneCombo", SceneTypeToString(selected))) {
			for (SceneType t : {SceneType::Game}) {
				bool is_selected = (selected == t);
				if (ImGui::Selectable(SceneTypeToString(t), is_selected)) {
					selected = t;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			GetGameState().SwitchScenes(selected);
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset")) {
			GetGameState().CurrentScene->Reset();
			GetGameState().CurrentScene->Init();
		}
	}

	if (ImGui::CollapsingHeader("entities", ImGuiTreeNodeFlags_DefaultOpen)) {
		auto players = GetEntitiesOfType<Player>();
		if (players.size() > 0) {
			auto *player = players[0];
			ImGui::Text("position: (%d, %d)", player->position.x, player->position.y);
			ImGui::Text("speed: (%.2f, %.2f)", player->speed.x, player->speed.y);
			ImGui::Text("remainder: (%.2f, %.2f)", player->remainder.x, player->remainder.y);
			ImGui::Text("direction: (%d, %d)", player->direction.x, player->direction.y);
			ImGui::Text("grounded: (%s)", player->IsGrounded() ? "true" : "false");

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 6));
			if (ImGui::Button("hurt")) {
				player->Hurt(CoordXY<float>::Zero);
			}
			ImGui::SameLine();
			if (ImGui::Button("kill")) {
				player->Kill();
			}
			ImGui::PopStyleVar();
		} else {
			ImGui::Text("player is dead\n");
		}
	}

	ImGui::End();
}
#endif

constexpr int kScreenWidth = 176;
constexpr int kScreenHeight = 128;

pti_desc pti_main(int argc, char *argv[]) {

	return (pti_desc) {
			.init_cb = init,
			.frame_cb = frame,
			.cleanup_cb = cleanup,
#if defined(PTI_DEBUG)
			.debug_cb = debug,
#endif
			.memory_size = _pti_kilobytes(512), /* 256KB */
			.width = kScreenWidth,
			.height = kScreenHeight,
	};
}