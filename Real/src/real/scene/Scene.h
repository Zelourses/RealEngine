#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "real/core/Timestep.h"


namespace Real {

	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name = "unnamed");

		void onUpdate(Timestep ts);
		void onViewportResize(glm::vec2 size);

	private:
		entt::registry registry;

		friend class Entity;
	};
}
