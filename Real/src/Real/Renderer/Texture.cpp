#include "repch.h"
#include "Texture.h"

#include "Renderer.h"
#include "platform/OpenGL/OpenGLTexture.h"

namespace Real {

	Ref<Texture2D> Texture2D::create(const std::string& path) {
		switch (Renderer::getRenderAPI()) {

		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
		}

		RE_CORE_ASSERT(false, "Unknown/Ubsupported Renderer API");
		return nullptr;
	}
}
