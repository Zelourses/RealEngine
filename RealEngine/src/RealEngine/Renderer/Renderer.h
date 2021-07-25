#pragma once

#include "RenderCommand.h"

namespace RealEngine {
	
	class Renderer {
	public:

		static void beginScene();
		static void endScene();

		static void submit(const std::shared_ptr<VertexArray>& vertexArray);
		
		inline static RendererAPI::API getRenderAPI() { return RendererAPI::getAPI(); }
	};
}
