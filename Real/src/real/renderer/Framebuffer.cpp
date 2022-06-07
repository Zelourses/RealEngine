#include "repch.h"
#include "Framebuffer.h"

#include "real/renderer/Renderer.h"
#include "platform/openGL/OpenGLFrameBuffer.h"

namespace Real {
	Ref<Framebuffer> Framebuffer::create(const FramebufferSpecification& spec) {
		switch (Renderer::getRenderAPI()) {
			case RendererAPI::API::None: RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
			case RendererAPI::API::OpenGL: return createRef<OpenGLFrameBuffer>(spec);
		}

		RE_CORE_ASSERT(false, "unsupported API");
		return nullptr;
	}
}