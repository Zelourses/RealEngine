#include "repch.h"
#include "Renderer.h"

#include "RenderCommand.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace Real {

	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

	void Renderer::beginScene(OrthographicCamera& camera) {
		sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}
	void Renderer::endScene() {

	}
	void Renderer::submit(const Ref<Shader>& shader,
		const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform) {

		// TEMP THINGS v2
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("viewProjection", sceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("transform", transform);
		
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}
