#pragma once

#include <variant>
#include <vector>

#include "registry.h"
#include "scene.h"

#include "solid.h"
#include "actor.h"

constexpr int kMaxEntities = 256;

namespace batteries {

	struct GameState {
		IScene *CurrentScene;

		void Reset(void) {
			CurrentScene->Reset();
		}

		void Tick(void) {
			CurrentScene->Update();
			CurrentScene->Render();
		}
	};

}// namespace batteries

batteries::IScene *Scene();