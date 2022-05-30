#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace Real {
	class Renderer2D {
	public:
		static void init();
		static void shutdown();


		static void beginScene(const OrthographicCamera& camera);
		static void endScene();

		static void flush();

		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f);

		// FIXME: tilingfactor for rotatedQuad
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture);

		struct Statistics {
			unsigned drawCalls = 0;
			unsigned quadCount = 0;

			unsigned getTotalVertexCount() { return quadCount * 4; }
			unsigned getTotalIndexCount() { return quadCount * 6; }
		};

		static Statistics getStats();
		static void       resetStats();

	private:
		static inline void reset();
		static inline void fillQuad(const glm::mat4& transform, const glm::vec4& color, float textureIndex, float tilingFactor, const std::array<glm::vec2, 4>& textureCoords);
	};
}
