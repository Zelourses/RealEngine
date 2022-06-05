#include "repch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "platform/OpenGL/OpenGLBuffer.h"

namespace Real {
	Ref<VertexBuffer> VertexBuffer::create(unsigned size) {
		switch (Renderer::getRenderAPI()) {
			case RendererAPI::API::None:
				RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet");
				return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(size);
		}

		RE_CORE_ASSERT(false, "Unknown/Unsupported Renderer API");
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::create(float* vertices, unsigned int size) {
		switch (Renderer::getRenderAPI()) {
			case RendererAPI::API::None:
				RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet");
				return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		RE_CORE_ASSERT(false, "Unknown/Unsupported Renderer API");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::create(unsigned int* indices, unsigned int count) {
		switch (Renderer::getRenderAPI()) {
			case RendererAPI::API::None:
				RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet");
				return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		RE_CORE_ASSERT(false, "Unknown/Unsupported Renderer API");
		return nullptr;
	}
}
