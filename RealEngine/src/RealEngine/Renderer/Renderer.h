#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace RealEngine {
	
	class Renderer {
	public:

		static void beginScene(OrthographicCamera& camera);
		static void endScene();

		static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
		
		inline static RendererAPI::API getRenderAPI() { return RendererAPI::getAPI(); }

	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* sceneData;
	};
}
