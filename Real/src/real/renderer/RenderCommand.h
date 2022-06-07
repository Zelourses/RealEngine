#pragma once

#include "RendererAPI.h"

namespace Real {

	class RenderCommand {
	public:

		inline static void init() { rendererAPI->init(); }
		inline static void setViewPort(unsigned int x, 
			unsigned int y, unsigned int width, unsigned int height) {
			rendererAPI->setViewPort(x,y,width,height);
		}

		inline static void setClearColor(const glm::vec4& color) {
			rendererAPI->setClearColor(color);
		}
		inline static void clear() {
			rendererAPI->clear();
		}
		
		inline static void drawIndexed(const Ref<VertexArray>& vertexArray) {
			rendererAPI->drawIndexed(vertexArray);
		}
		inline static void drawIndexed(const Ref<VertexArray>& vertexArray, unsigned indexCount) {
			rendererAPI->drawIndexed(vertexArray, indexCount);
		}
	private:
		static RendererAPI* rendererAPI;
	};
	
}
