#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Real {
	class RendererAPI {
	public:
		enum class API {
			None   = 0,
			OpenGL = 1,
		};

		virtual void init() = 0;

		virtual void setViewPort(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear()                               = 0;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray)                      = 0;
		virtual void drawIndexed(const Ref<VertexArray>& vertexArray, unsigned indexCount) = 0;

		inline static API getAPI() { return currentAPI; }

		virtual ~RendererAPI() = default;

	private:
		static API currentAPI;
	};
}
