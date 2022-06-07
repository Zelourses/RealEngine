#include "repch.h"
#include "VertexArray.h"

#include "Real/Renderer/Renderer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"

namespace Real {

	Ref<VertexArray> VertexArray::create() {
		switch (Renderer::getRenderAPI()) {

		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
		}

		RE_CORE_ASSERT(false, "Unknown/Unsupported Renderer API");
		return nullptr;
	}
}
