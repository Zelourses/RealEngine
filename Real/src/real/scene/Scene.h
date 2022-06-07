#pragma once

#include <entt/entt.hpp>
#include "real/core/Timestep.h"

namespace Real {

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name = "unnamed");

		void onUpdate(Timestep ts);

	private:
		entt::registry registry;

		friend class Entity;
	};
}
