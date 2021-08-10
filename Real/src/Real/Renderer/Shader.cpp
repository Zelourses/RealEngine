#include "repch.h"
#include "Shader.h"

#include "Renderer.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace Real {
	Shader* Shader::create(const std::string& filePath) {
		switch (Renderer::getRenderAPI()) {

		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLShader(filePath);
		}

		RE_CORE_ASSERT(false, "Unknown/Ubsupported Renderer API");
		return nullptr;
	}

	Shader* Shader::create(const std::string& vertexSrc, const std::string& pixelSrc) {
		switch (Renderer::getRenderAPI()) {

		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLShader(vertexSrc, pixelSrc);
		}

		RE_CORE_ASSERT(false, "Unknown/Ubsupported Renderer API");
		return nullptr;
	}
}
