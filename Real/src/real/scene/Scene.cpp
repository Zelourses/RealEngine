#include "repch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include "Entity.h"
#include "Components.h"
#include "real/renderer/Renderer2D.h"

namespace Real {
	Scene::Scene() {
	}

	Scene::~Scene() {
	}
	Entity Scene::createEntity(const std::string& name /*="unnamed"*/) {
		Entity entity = { registry.create(), this };
		entity.add<TransformComponent>();
		entity.add<TagComponent>(name);
		return entity;
	}
	void Scene::onUpdate(Timestep ts) {
		auto&& group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto&& entity : group) {
			auto&& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::drawQuad(transform,sprite);
		}
	}
}
