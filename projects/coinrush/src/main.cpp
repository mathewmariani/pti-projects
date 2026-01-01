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

static void load(void) {
	GameStateInit();
	batteries::reload();

	ChangeLevels();
}

static void init(void) {
	batteries::init();
	tileset = batteries::tileset("assets/tilemap.ase");
	tilemap = batteries::tilemap("assets/tilemap.ase");
	bitmap_bullet = batteries::sprite("assets/bullet.ase");
	bitmap_coin = batteries::sprite("assets/coin.ase");
	bitmap_player = batteries::sprite("assets/dog.ase");
	bitmap_goomba = batteries::sprite("assets/zombie.ase");
	bitmap_heart = batteries::sprite("assets/heart.ase");
	bitmap_platform = batteries::sprite("assets/platform.ase");
	bitmap_font = batteries::sprite("assets/font.ase");
	bitmap_fx_collect1 = batteries::sprite("assets/collect.ase");
	bitmap_fx_collect2 = batteries::sprite("assets/collect2.ase");
	bitmap_shooter = batteries::sprite("assets/cannon.ase");
	bitmap_door = batteries::sprite("assets/door.ase");

	GetGameState().levels = {
			batteries::tilemap("assets/levels/01.ase"),
			batteries::tilemap("assets/levels/02.ase"),
			batteries::tilemap("assets/levels/03.ase"),
			batteries::tilemap("assets/levels/04.ase"),
			batteries::tilemap("assets/levels/05.ase"),
			batteries::tilemap("assets/levels/06.ase"),
	};

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
	auto &gameState = GetGameState();
	if (pti_down(PTI_DBG)) {
		GameStateReset();
		load();
		return;
	}

	if (gameState.PlayerIsDead) {
		gameState.ResetTimer += PTI_DELTA;
		if (gameState.ResetTimer >= kDeathResetTimer) {
			gameState.Deaths++;
			ChangeLevels();
			return;
		}
	}

	GameStateTick();

	pti_cls(0xffef7d57);
	pti_map(0, 0);

	RenderAllEntities();

	DoShake();

	/* render ui */
	char buffer[100];

	// coin counter
	std::snprintf(buffer, sizeof(buffer), "Coins: %d\n", gameState.Coins);
	std::string coin_str(buffer);
	pti_print(coin_str.c_str(), 8, 4);

	// death counter
	std::snprintf(buffer, sizeof(buffer), "Deaths: %d\n", gameState.Deaths);
	std::string death_str(buffer);
	pti_print(death_str.c_str(), 8, 16);

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
	ImGui::Begin("Zombies", NULL, ImGuiWindowFlags_AlwaysAutoResize);

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

	if (ImGui::CollapsingHeader("entities", ImGuiTreeNodeFlags_DefaultOpen)) {
		auto players = GetEntitiesOfType<Player>();
		if (players.size() > 0) {
			auto *player = players[0];
			ImGui::Text("position: (%d, %d)", player->position.x, player->position.y);
			ImGui::Text("speed: (%.2f, %.2f)", player->speed.x, player->speed.y);
			// ImGui::Text("grounded: (%s)", player->IsGrounded() ? "true" : "false");

			if (ImGui::Button("hurt")) {
				std::printf("hurt\n");
			}
			if (ImGui::Button("kill")) {
				std::printf("kill\n");
			}
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