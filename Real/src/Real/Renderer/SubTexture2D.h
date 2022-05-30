#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Real {
	class SubTexture2D {
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const std::array<glm::vec2, 4>& getTexCoords() const { return texCoords; }
		const Ref<Texture2D> getTexture() const { return texture; }

		static Ref<SubTexture2D> createFromCoods(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize);

	private:
		Ref<Texture2D> texture;

		std::array<glm::vec2,4> texCoords;
	};
}
