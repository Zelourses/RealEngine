#include "repch.h"
#include "VertexArray.h"

#include "Real/Renderer/Renderer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"

namespace Real {

	VertexArray* VertexArray::create() {
		switch (Renderer::getRenderAPI()) {

		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
		}

		RE_CORE_ASSERT(false, "Unknown/Ubsupported Renderer API");
		return nullptr;
	}
}
