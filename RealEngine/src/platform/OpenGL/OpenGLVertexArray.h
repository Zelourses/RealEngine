#pragma once

#include "RealEngine/Renderer/VertexArray.h"

namespace RealEngine{
	class OpenGLVertexArray : public VertexArray{
	public:
		void bind() const override;

		void unbind() const override;

		void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		~OpenGLVertexArray();
		OpenGLVertexArray();

		inline const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override { return indexBuffer; }
		inline const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override { return vertexBuffers; }

	private:
		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
		std::shared_ptr<IndexBuffer> indexBuffer;
		uint32_t rendererId;
	};
}
