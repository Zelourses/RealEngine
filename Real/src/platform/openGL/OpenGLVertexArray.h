#pragma once

#include "Real/Renderer/VertexArray.h"

namespace Real {
	class OpenGLVertexArray : public VertexArray {
	public:
		void bind() const override;

		void unbind() const override;

		void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		~OpenGLVertexArray();
		OpenGLVertexArray();

		inline const Ref<IndexBuffer>&               getIndexBuffer() const override { return indexBuffer; }
		inline const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override { return vertexBuffers; }

	private:
		std::vector<Ref<VertexBuffer>> vertexBuffers;
		Ref<IndexBuffer>               indexBuffer;
		unsigned int                   rendererId;
	};
}
