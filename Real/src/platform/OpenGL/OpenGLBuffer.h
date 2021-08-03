#pragma once
#include <Real/Renderer/Buffer.h>
namespace Real {
	class OpenGLVertexBuffer : public VertexBuffer{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		void bind() const override;
		void unbind() const override;

		void setLayout(const BufferLayout& layout_) override { layout = layout_; }
		const BufferLayout& getLayout() const override { return layout; }

		~OpenGLVertexBuffer() override;

	private:
		uint32_t rendererId;
		BufferLayout layout;
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

