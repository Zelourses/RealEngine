#pragma once

#include "Real/Renderer/RendererAPI.h"

namespace Real {

	class OpenGLRendererAPI : public RendererAPI{
	public:
		virtual void init() override;
		
		void setClearColor(const glm::vec4& color) override;
		void clear() override;
		
		void drawIndexed(const Ref<VertexArray>& vertexArray) override;

	};
	
}
