#pragma once

#include "OrthographicCamera.h"
#include "Camera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "EditorCamera.h"
#include "real/scene/Components.h"

namespace Real {
	class Renderer2D {
	public:
		static void init();
		static void shutdown();


		static void beginScene(const OrthographicCamera& camera); // TODO:remove
		static void beginScene(const EditorCamera& camera);
		static void beginScene(const Camera& camera, const glm::mat4& transform);
		static void endScene();

		static void flush();

		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f);

		static void drawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId = -1);
		static void drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, int entityId = -1);

		// FIXME: tilingfactor for rotatedQuad
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture);

		static void drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityId);

		struct Statistics {
			unsigned drawCalls = 0;
			unsigned quadCount = 0;

			unsigned getTotalVertexCount() const { return quadCount * 4; }
			unsigned getTotalIndexCount() const { return quadCount * 6; }
		};

		static Statistics getStats();
		static void		  resetStats();

	private:
		static inline void reset();
		static inline void fill(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, int entityId);
		static inline void fill(const glm::mat4& transform, const glm::vec4& color, const Ref<SubTexture2D>& subTexture, float tilingFactor, int entityId);
		static inline void fillQuad(const glm::mat4& transform, const glm::vec4& color, float textureIndex, float tilingFactor, const std::array<glm::vec2, 4>& textureCoords, int entityId);

	};
}
