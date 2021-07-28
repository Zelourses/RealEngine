#include "repch.h"
#include "Renderer.h"

#include "RenderCommand.h"

namespace RealEngine {

	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

	void Renderer::beginScene(OrthographicCamera& camera) {
		sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}
	void Renderer::endScene() {

	}
	void Renderer::submit(const std::shared_ptr<Shader>& shader, 
		const std::shared_ptr<VertexArray>& vertexArray) {

		// TEMP THINGS
		shader->bind();
		shader->uploadUniformMat4("viewProjection", sceneData->viewProjectionMatrix);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}
