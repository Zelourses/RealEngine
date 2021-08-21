#pragma once

#include "Real/Renderer/RendererAPI.h"

namespace Real {

	class OpenGLRendererAPI : public RendererAPI{
	public:
		void init() override;

		void setViewPort(unsigned x, unsigned y, unsigned width, unsigned height) override;
		
		void setClearColor(const glm::vec4& color) override;
		void clear() override;
		
		void drawIndexed(const Ref<VertexArray>& vertexArray) override;

	};
	
}
