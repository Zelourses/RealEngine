#include "repch.h"
#include "VertexArray.h"

#include "RealEngine/Renderer/Renderer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"

namespace RealEngine {

	VertexArray* VertexArray::create() {
		switch (Renderer::getRenderAPI()) {

		case RendererAPI::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}

		RE_CORE_ASSERT(false, "Unknown/Ubsupported Renderer API");
		return nullptr;
	}
}
