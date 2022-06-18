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

		using instantiateScriptType = std::add_pointer<ScriptableEntity*()>::type;
		using destroyScriptType		= std::add_pointer<void(NativeScriptComponent*)>::type;

		instantiateScriptType instantiateScript;
		destroyScriptType	  destroyScript;

		template <typename EntityType>
		void bind() {
			instantiateScript = []() -> ScriptableEntity* { return new EntityType(); };
			destroyScript	  = [](NativeScriptComponent* component) { delete component->instance;  /*for debug*/component->instance = nullptr; };
		}
	};

}
