#pragma once

#include <glm/glm.hpp>

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
}
