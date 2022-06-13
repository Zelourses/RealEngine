#pragma once

#include <glm/glm.hpp>
#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Real {

	struct TransformComponent {
		glm::mat4 transform = glm::mat4(1.0f);
				  operator glm::mat4&() { return transform; }
	};

	struct SpriteRendererComponent {
		glm::vec4 color = glm::vec4(1.0f);
				  operator glm::vec4&() { return color; }
	};
	struct TagComponent {
		std::string tag;
					operator std::string&() { return tag; }
	};

	struct CameraComponent {
		SceneCamera camera;
		bool		primary			 = true;
		bool		fixedAspectRatio = false;
					operator Camera&() { return camera; }
	};

	struct NativeScriptComponent {
		ScriptableEntity* instance = nullptr;

		std::function<void()> instantiateFunc;
		std::function<void()> destroyInstanceFunc;

		std::function<void(ScriptableEntity*)> onCreateFunc;
		std::function<void(ScriptableEntity*, Timestep)> onUpdateFunc;
		std::function<void(ScriptableEntity*)> onDestroyFunc;

		template <typename EntityType>
		void bind() {
			instantiateFunc		= [this]() { instance = new EntityType(); };
			destroyInstanceFunc = [this]() { delete static_cast<EntityType*>(instance);  /*for debug*/instance = nullptr; };

			onCreateFunc  = [](ScriptableEntity* instance) { (static_cast<EntityType*>(instance))->onCreate(); };
			onUpdateFunc  = [](ScriptableEntity* instance, Timestep ts) { (static_cast<EntityType*>(instance))->onUpdate(ts); };
			onDestroyFunc = [](ScriptableEntity* instance) { (static_cast<EntityType*>(instance))->onDestroy(); };
		}
	};

}
