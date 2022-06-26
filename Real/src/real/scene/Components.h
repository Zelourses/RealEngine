#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Real {

	struct TransformComponent {
		glm::vec3 translation = glm::vec3(0.0f);
		glm::vec3 rotation	  = glm::vec3(0.0f);
		glm::vec3 scale		  = glm::vec3(1.0f);

		glm::mat4 transform() const {
			glm::mat4 rotate = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), translation)
				 * rotate
				 * glm::scale(glm::mat4(1.0f), scale);
		}
	};

	struct SpriteRendererComponent {
		glm::vec4 color = glm::vec4(1.0f);
	};
	struct TagComponent {
		std::string tag;
	};

	struct CameraComponent {
		SceneCamera camera;
		bool		primary			 = true;
		bool		fixedAspectRatio = false;
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
