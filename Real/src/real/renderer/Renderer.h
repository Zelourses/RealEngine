#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Real {
	
	class Renderer {
	public:

		static void init();
		static void onWindowResize(unsigned int width, unsigned int height);

		static void beginScene(OrthographicCamera& camera);
		static void endScene();

		static void submit(const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f));
		
		inline static RendererAPI::API getRenderAPI() { return RendererAPI::getAPI(); }

	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* sceneData;
	};
}
