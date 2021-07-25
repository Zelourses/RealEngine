#pragma once

#include "RealEngine/Renderer/RendererAPI.h"

namespace RealEngine {

	class OpenGLRendererAPI : public RendererAPI{
	public:
		void setClearColor(const glm::vec4& color) override;
		void clear() override;
		
		void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;

	};
	
}
