#pragma once
#include <Real/Renderer/Buffer.h>
namespace Real {
	class OpenGLVertexBuffer : public VertexBuffer{
	public:
		OpenGLVertexBuffer(unsigned size);
		OpenGLVertexBuffer(float* vertices, unsigned size);

		void bind() const override;
		void unbind() const override;

		void setData(const void* data, unsigned size) override;

		void setLayout(const BufferLayout& layout_) override { layout = layout_; }
		const BufferLayout& getLayout() const override { return layout; }

		~OpenGLVertexBuffer() override;

	private:
		unsigned int rendererId;
		BufferLayout layout;
	};


	class OpenGLIndexBuffer : public IndexBuffer{
	public:
		OpenGLIndexBuffer(unsigned int* indices, unsigned int size);
		void bind() const override;
		void unbind() const override;

		unsigned int getCount() const override { return count; }

		~OpenGLIndexBuffer() override;

	private:
		unsigned int rendererId;
		unsigned int count;

	};
}

