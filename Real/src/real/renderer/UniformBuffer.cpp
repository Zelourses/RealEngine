#include "repch.h"
#include "UniformBuffer.h"

#include "real/renderer/Renderer.h"
#include "platform/openGL/OpenGLUniformBuffer.h"

namespace Real {
	Ref<UniformBuffer> UniformBuffer::create(size_t size, unsigned binding) {
		switch (Renderer::getRenderAPI()) {
			case RendererAPI::API::None: RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
			case RendererAPI::API::OpenGL: return createRef<OpenGLUniformBuffer>(size, binding);
		}
		RE_CORE_ASSERT(false, "unsupported API");
		return nullptr;
	}
}
