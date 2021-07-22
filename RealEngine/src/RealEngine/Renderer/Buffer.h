#pragma once

namespace RealEngine {

	class VertexBuffer {
	public:

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static VertexBuffer* create(float* vertices, uint32_t size);

		
		virtual ~VertexBuffer() {}
	};

	class IndexBuffer {
	public:
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		//how many indices in buffer
		virtual uint32_t getCount() const = 0;


		static IndexBuffer* create(uint32_t* indices, uint32_t size);

		virtual ~IndexBuffer() {}
		
	};
}
