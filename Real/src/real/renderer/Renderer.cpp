#include "repch.h"
#include "Renderer.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace Real {

	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

	void Renderer::init() {
		RenderCommand::init();
		Renderer2D::init();
	}

	void Renderer::onWindowResize(unsigned width, unsigned height) {
		RenderCommand::setViewPort(0,0,width, height);
	}

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
		shader->setMat4("viewProjection", sceneData->viewProjectionMatrix);
		shader->setMat4("transform", transform);
		
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}
