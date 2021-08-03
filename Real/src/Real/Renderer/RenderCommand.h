#pragma once

#include "RendererAPI.h"

namespace Real {

	class RenderCommand {
	public:

		inline static void setClearColor(const glm::vec4& color) {
			rendererAPI->setClearColor(color);
		}
		inline static void clear() {
			rendererAPI->clear();
		}
		
		inline static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
			rendererAPI->drawIndexed(vertexArray);
		}
	private:
		static RendererAPI* rendererAPI;
	};
	
}
