#include "repch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include "Entity.h"
#include "Components.h"
#include "real/renderer/Renderer2D.h"

namespace Real {

	Scene::Scene() 
	: viewportSize(1.0f,1.0f) {
		registry.on_construct<CameraComponent>().connect<&Scene::onCameraComponentAdd>(*this);
	}

	Scene::~Scene() {
	}
	Entity Scene::createEntity(const std::string& name /*="unnamed"*/) {
		Entity entity = {registry.create(), this};
		entity.add<TransformComponent>();
		entity.add<TagComponent>(name);
		return entity;
	}
	void Scene::onUpdateEditor(Timestep ts, EditorCamera& cam) {
		Renderer2D::beginScene(cam);

		auto&& group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto&& entity: group) {
			const auto&& [entityTransformComponent, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::drawSprite(entityTransformComponent.transform(), sprite, static_cast<int>(entity));
		}

		Renderer2D::endScene();
	}
	void Scene::onUpdateRuntime(Timestep ts) {
		//Update Scripts
		{
			registry.view<NativeScriptComponent>().each([ts, this](auto&& entity, auto&& nsc) {
				//TODO: move to something like Scene::onScenePlay()
				if (!nsc.instance) {
					nsc.instance		 = nsc.instantiateScript();
					nsc.instance->entity = Entity{entity, this};
					nsc.instance->onCreate();
				}

				nsc.instance->onUpdate(ts);
				//TODO: call onDestroy in something like Scene::onSceneStop();
			});
		}

		auto cameraView = registry.view<TransformComponent, CameraComponent>();

		auto&& cam = std::ranges::find_if(cameraView, [&cameraView](auto&& val) {
			auto&& [transform, camera] = cameraView.get<TransformComponent, CameraComponent>(val);
			return camera.primary;
		});

		if (cam != cameraView.end()) {
			auto&& [transformComponent, camera] = cameraView.get<TransformComponent, CameraComponent>(*cam);
			auto&& transform					= transformComponent.transform();

			Renderer2D::beginScene(camera.camera, transform);


			auto&& group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto&& entity: group) {
				auto&& [entityTransformComponent, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::drawQuad(entityTransformComponent.transform(), sprite.color);
			}

			Renderer2D::endScene();
		}
	}
	void Scene::onViewportResize(glm::vec2 size) {
		viewportSize = size;

		auto&& view = registry.view<CameraComponent>();
		for (auto&& entity: view) {
			CameraComponent& camera = view.get<CameraComponent>(entity);
			if (!camera.fixedAspectRatio) {
				camera.camera.setViewPortSize(size);
			}
		}
	}
	void Scene::destoryEntity(Entity entity) {
		registry.destroy(entity);
	}
	void Scene::onCameraComponentAdd(entt::registry& registry, entt::entity entity) {
		onViewportResize(viewportSize);
	}

	Entity Scene::getPrimaryCamera() {
		auto view = registry.view<CameraComponent>();
		for (auto&& v: view) {
			auto&& cam = view.get<CameraComponent>(v);
			if (cam.primary)
				return {v, this};
		}
		return {};
	}
}
