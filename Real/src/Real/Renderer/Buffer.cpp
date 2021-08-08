#include "repch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "platform/OpenGL/OpenGLBuffer.h"

namespace Real {

	VertexBuffer* VertexBuffer::create(float* vertices, unsigned int size) {
		switch (Renderer::getRenderAPI()) {

		case RendererAPI::API::None: 
			RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices,size);
		}

		RE_CORE_ASSERT(false, "Unknown/Ubsupported Renderer API");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::create(unsigned int* indices, unsigned int size) {
		switch (Renderer::getRenderAPI()) {

		case RendererAPI::API::None:
			RE_CORE_ASSERT(false, "RendererAPI::None is not supported yet"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, size);
		}

		RE_CORE_ASSERT(false, "Unknown/Ubsupported Renderer API");
		return nullptr;
	}
}
