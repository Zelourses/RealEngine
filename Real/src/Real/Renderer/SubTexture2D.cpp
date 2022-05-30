#include "repch.h"

#include "SubTexture2D.h"

namespace Real {
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
	    : texture(texture) {
		texCoords[0] = {min.x, min.y};
		texCoords[1] = {max.x, min.y};
		texCoords[2] = {max.x, max.y};
		texCoords[3] = {min.x, max.y};
	}
	Ref<SubTexture2D> SubTexture2D::createFromCoods(const Ref<Texture2D>& texture,
	                                                const glm::vec2&      coords,
	                                                const glm::vec2&      spriteSize) {
		unsigned width  = texture->getWidth();
		unsigned height = texture->getHeight();

		glm::vec2 min = {(coords.x * spriteSize.x) / width,
		            (coords.y * spriteSize.y) / height};
		glm::vec2 max = {((coords.x + 1) * spriteSize.x) / width, ((coords.y + 1) * spriteSize.y) / height};
		return createRef<SubTexture2D>(texture, min, max);
	}
}