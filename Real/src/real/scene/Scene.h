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
		void destoryEntity(Entity entity);

		Entity getPrimaryCamera();

	private:
		void onCameraComponentAdd(entt::registry& registry, entt::entity entity);

		entt::registry registry;
		glm::vec2 viewportSize;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}
