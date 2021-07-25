#include "repch.h"
#include "Renderer.h"

#include "RenderCommand.h"

namespace RealEngine {

	void Renderer::beginScene() {

	}
	void Renderer::endScene() {

	}
	void Renderer::submit(const std::shared_ptr<VertexArray>& vertexArray) {
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}