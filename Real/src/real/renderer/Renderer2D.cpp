#include "repch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"
#include "UniformBuffer.h"


namespace Real {

#pragma pack(push, 1)
	struct QuadVertex {
		glm::vec3 position;
		glm::vec2 textureCoords;
		glm::vec4 color;
		float	  textureindex;
		float	  tilingFactor;

		//Editor only
		int entityId;
	};
#pragma pack(pop)

	struct Renderer2DData {
		static const unsigned maxQuadsCount	  = 10'000;
		static const unsigned maxVertices	  = maxQuadsCount * 4; // becuase rectangle is 4 quads
		static const unsigned maxIndices	  = maxQuadsCount * 6; // one rectangle is 6 indices (to draw 2 triangles)
		static const unsigned maxTextureSlots = 32;				   // Some GPU's have that (and mine too)
																   // TODO: RenderCapabilities, see:
																   // https://gamedev.stackexchange.com/questions/30043/how-many-textures-can-usually-i-bind-at-once

		Ref<VertexArray>  quadVA;
		Ref<VertexBuffer> quadVB;
		Ref<Shader>		  textureShader;
		Ref<Texture2D>	  whiteTexture;

		unsigned	quadIndexCount	  = 0;
		QuadVertex* quadVertexBase	  = nullptr;
		QuadVertex* quadVertexPointer = nullptr;

		// FIXME: asset id(guid), not OpenGL texture id
		std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
		unsigned									textureSlotIndex = 1; // 0 is white texture

		glm::vec4 quadVertexPos[4] = {
			{-0.5f, -0.5f, 0.0f, 1.0f},
			{0.5f, -0.5f, 0.0f, 1.0f},
			{0.5f, 0.5f, 0.0f, 1.0f},
			{-0.5f, 0.5f, 0.0f, 1.0f}};

		std::array<glm::vec2, 4> defaultTexCoords = {
			// This is actually a hint to compiler
			glm::vec2{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}};

		Renderer2D::Statistics stats;

		struct CameraData {
			glm::mat4 viewProjection;
		};

		CameraData		   cameraBuffer{glm::mat4(0.0f)};
		Ref<UniformBuffer> cameraUniformBuffer{};
	};

	static Renderer2DData data;


	void Renderer2D::init() {
		data.quadVA = Real::VertexArray::create();

		data.quadVB = VertexBuffer::create(data.maxVertices * sizeof(QuadVertex));
		data.quadVB->setLayout({
			{Real::ShaderDataType::Float3, "position"},
			{Real::ShaderDataType::Float2, "textCoordinates"},
			{Real::ShaderDataType::Float4, "color"},
			{Real::ShaderDataType::Float, "textureIndex"},
			{Real::ShaderDataType::Float, "tilingFactor"},
			{Real::ShaderDataType::Int, "entityId"},
		});
		data.quadVA->addVertexBuffer(data.quadVB);

		data.quadVertexBase = new QuadVertex[data.maxVertices];

		auto* quadIndices = new unsigned[data.maxIndices];


		unsigned offset = 0;
		for (unsigned i = 0; i < data.maxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Real::Ref<Real::IndexBuffer> quadIB;
		quadIB = Real::IndexBuffer::create(quadIndices, data.maxIndices);
		data.quadVA->setIndexBuffer(quadIB);

		data.whiteTexture		  = Texture2D::create(1, 1);
		unsigned whiteTextureData = 0xffffffff;
		data.whiteTexture->setData(&whiteTextureData, sizeof(unsigned));

		int samplers[data.maxTextureSlots];
		std::iota(samplers, samplers + data.maxTextureSlots, 0);

		data.textureShader = Shader::create("assets/shaders/Texture.glsl");
		data.textureShader->bind();
		data.textureShader->setIntArray("uTextures", samplers, data.maxTextureSlots);

		data.textureSlots[0]	 = data.whiteTexture;
		data.cameraUniformBuffer = UniformBuffer::create(sizeof(Renderer2DData::CameraData), 0);

		// Actually it's very dangerous to do something like that, because we are not 100% sure that
		//  OpenGL function is actually read all lines here
		delete[] quadIndices;
	}

	void Renderer2D::shutdown() {}

	void Renderer2D::beginScene(const OrthographicCamera& camera) {
		RE_PROFILE_FUNCTION();
		data.textureShader->bind();
		data.textureShader->setMat4("viewProjection", camera.getViewProjectionMatrix());

		reset();
	}

	void Renderer2D::beginScene(const EditorCamera& camera) {
		RE_PROFILE_FUNCTION();

		data.cameraBuffer.viewProjection = camera.viewProjection();
		data.cameraUniformBuffer->setData(&data.cameraBuffer, sizeof(Renderer2DData::cameraBuffer));

		reset();
	}

	void Renderer2D::beginScene(const Camera& camera, const glm::mat4& transform) {
		RE_PROFILE_FUNCTION();

		data.cameraBuffer.viewProjection = camera.projection() * glm::inverse(transform);
		data.cameraUniformBuffer->setData(&data.cameraBuffer, sizeof(Renderer2DData::cameraBuffer));

		reset();
	}

	void Renderer2D::endScene() {
		RE_PROFILE_FUNCTION();

		unsigned dataSize = static_cast<unsigned>((uint8_t*)data.quadVertexPointer - (uint8_t*)data.quadVertexBase);
		data.quadVB->setData(data.quadVertexBase, dataSize);

		flush();
	}

	void Renderer2D::flush() {
		RE_PROFILE_FUNCTION();
		// TODO: Bind textures
		for (unsigned i = 0; i < data.textureSlotIndex; i++) {
			data.textureSlots[i]->bind(i);
		}

		data.textureShader->bind();
		RenderCommand::drawIndexed(data.quadVA, data.quadIndexCount);

		++data.stats.drawCalls;
	}


	inline void Renderer2D::fillQuad(const glm::mat4&				 transform,
									 const glm::vec4&				 color,
									 float							 textureIndex,
									 float							 tilingFactor,
									 const std::array<glm::vec2, 4>& textureCoords,
									 int							 entityId) {
		// Does this thing belongs here?
		if (Renderer2DData::maxIndices <= data.quadIndexCount) {
			Renderer2D::endScene();
			Renderer2D::reset();
		}

		// Counterclockwise way, start in left down corner

		// lef down
		// right down
		// right up
		// left up
		for (int i = 0; i < 4; ++i) {
			data.quadVertexPointer->position	  = transform * data.quadVertexPos[i];
			data.quadVertexPointer->color		  = color;
			data.quadVertexPointer->textureCoords = textureCoords[i];
			data.quadVertexPointer->textureindex  = textureIndex;
			data.quadVertexPointer->tilingFactor  = tilingFactor;
			data.quadVertexPointer->entityId	  = entityId;
			data.quadVertexPointer++;
		}
		data.quadIndexCount += 6;
	}

	static float getTextureIndex(const Ref<Texture2D>& texture) {
		float textureIndex = 0.0f;

		auto&& last	  = data.textureSlots.begin() + data.textureSlotIndex;
		auto&& result = std::find_if(data.textureSlots.begin()++, last, [&texture](auto&& val) {
			return *val.get() == *texture.get();
		});


		textureIndex = static_cast<float>(std::distance(data.textureSlots.begin(), result));

		if (result == last) {
			data.textureSlots[data.textureSlotIndex++] = texture;
		}

		return textureIndex;
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		RE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		fill(transform, color, data.whiteTexture, 1.0f, -1);
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId /*=-1*/) {
		RE_PROFILE_FUNCTION();
		fill(transform, color, data.whiteTexture, 1.0f, entityId);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor /* = 1.0f*/) {
		RE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		drawQuad(transform, texture, tilingFactor);
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor /* = 1.0f*/, int entityId /*=-1*/) {
		RE_PROFILE_FUNCTION();
		constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
		fill(transform, color, texture, tilingFactor, entityId);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor /* = 1.0f*/) {
		RE_PROFILE_FUNCTION();

		constexpr glm::vec4 color	  = {1.0f, 1.0f, 1.0f, 1.0f};
		glm::mat4			transform = glm::translate(glm::mat4(1.0f), position)
							* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		fill(transform, color, subTexture, tilingFactor, -1);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		RE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
							* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		fill(transform, color, data.whiteTexture, 1.0f, -1);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture) {
		RE_PROFILE_FUNCTION();

		auto	  color		= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
							* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		fill(transform, color, texture, 1.0f, -1);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture) {
		RE_PROFILE_FUNCTION();

		auto	  color		= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
							* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		fill(transform, color, subTexture, 1.0f, -1);
	}

	void Renderer2D::drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityId) {
		RE_PROFILE_FUNCTION();
		drawQuad(transform, src.color, entityId);
	}

	Renderer2D::Statistics Renderer2D::getStats() {
		return data.stats;
	}

	void Renderer2D::resetStats() {
		// or memset
		data.stats.drawCalls = 0;
		data.stats.quadCount = 0;
	}

	inline void Renderer2D::reset() {
		data.quadIndexCount	   = 0;
		data.quadVertexPointer = data.quadVertexBase;
		data.textureSlotIndex  = 1;
	}

	inline void Renderer2D::fill(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor, int entityId) {
		fillQuad(transform, color, getTextureIndex(texture), tilingFactor, data.defaultTexCoords, entityId);
		++data.stats.quadCount;
	}

	inline void Renderer2D::fill(const glm::mat4& transform, const glm::vec4& color, const Ref<SubTexture2D>& subTexture, float tilingFactor, int entityId) {
		fillQuad(transform, color, getTextureIndex(subTexture->getTexture()), tilingFactor, subTexture->getTexCoords(), entityId);
		++data.stats.quadCount;
	}
}
