#include "repch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "platform/OpenGL/OpenGLBuffer.h"

namespace RealEngine {

	VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size) {
		switch (Renderer::getRenderAPI()) {

		case RendererAPI::API::None: 
			RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices,size);
		}

		RE_CORE_ASSERT(false, "Unknown/Ubsupported Renderer API");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t size) {
		switch (Renderer::getRenderAPI()) {

		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, size);
		}

		RE_CORE_ASSERT(false, "Unknown/Ubsupported Renderer API");
		return nullptr;
	}
}
