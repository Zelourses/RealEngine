#pragma once
#include <RealEngine/Renderer/Buffer.h>
namespace RealEngine {
	class OpenGLVertexBuffer : public VertexBuffer{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		void bind() const override;
		void unbind() const override;

		~OpenGLVertexBuffer() override;

	private:
		uint32_t rendererId;
	};


	class OpenGLIndexBuffer : public IndexBuffer{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		void bind() const override;
		void unbind() const override;

		uint32_t getCount() const override { return count; }

		~OpenGLIndexBuffer() override;

	private:
		uint32_t rendererId;
		uint32_t count;

	};
}

