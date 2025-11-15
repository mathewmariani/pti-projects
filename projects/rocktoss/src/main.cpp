/* engine */
#include "pti/pti.h"

#include "bank.h"
#include "gamestate.h"
#include "batteries/assets.h"
#include "batteries/registry.h"
#include "batteries/helper.h"
#include "batteries/juice.h"

#include <string>

#if defined(PTI_DEBUG)
#include "imgui/imgui.h"
bool show_overlays = false;
#endif

#define XPOS(x) (x * kTileSize)
#define YPOS(y) (y * kTileSize)

static void load(void) {
	// copties the bank contents into memory
	batteries::reload();
	GameStateInit();
}

static void init(void) {
	batteries::init();
	tileset = batteries::tileset("assets/tilemap.ase");
	tilemap = batteries::tilemap("assets/tilemap.ase");
	bitmap_player = batteries::sprite("assets/dog.ase");
	bitmap_bullet = batteries::sprite("assets/bullet.ase");
	bitmap_font = batteries::sprite("assets/font.ase");

	pti_set_tilemap(tilemap);
	pti_set_tileset(tileset);
	pti_set_font(bitmap_font);

	load();

	/* graphics state: */
	// pti_dither(0x5a5a);
	pti_clip(0, 0, kScreenWidth, kScreenHeight);
}

static void cleanup(void) {
	/* ... */
}

static void frame(void) {
	GameStateTick();
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
		case SceneType::Menu:
			return "Menu";
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

		static SceneType selected = SceneType::Menu;
		if (ImGui::BeginCombo("##SceneCombo", SceneTypeToString(selected))) {
			for (SceneType t : {SceneType::Menu, SceneType::Game}) {
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
			batteries::reload();
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