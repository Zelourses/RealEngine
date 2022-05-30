#include "repch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Real {

	static GLenum shaderDataTypeToOpenGLBaseType(SDT type) {
		switch (type) {
			case SDT::None: break;

			case SDT::Float:	//fallthrough
			case SDT::Float2:	//fallthrough
			case SDT::Float3:	//fallthrough
			case SDT::Float4:	//fallthrough
			case SDT::Mat3:		//fallthrough
			case SDT::Mat4:		return GL_FLOAT;

			case SDT::Int:		//fallthrough
			case SDT::Int2:		//fallthrough
			case SDT::Int3:		//fallthrough
			case SDT::Int4:		return GL_INT;

			case SDT::Bool:		return GL_BOOL;
		}
		RE_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	void OpenGLVertexArray::bind() const {
		glBindVertexArray(rendererId);
	}
	void OpenGLVertexArray::unbind() const {
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		RE_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "VertexBuffer has no layout");
		
		glBindVertexArray(rendererId);
		vertexBuffer->bind();

		unsigned int i = 0;
		const auto& layout = vertexBuffer->getLayout();
		for (const auto& l : layout) {
			/*
			* This line existence is determined by compiler warning that it's wrong
			*  to cast unsigned int to unsigned int* \/
			*  (conversion from 'const unsigned int' to 'const unsigned int *' of greater size)
			*  to get rid of this error I need to do something not so pretty
			*/
			const void* offset = reinterpret_cast<uint8_t*>(0) + (l.offset);
			
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i,
				l.getComponentCount(),
				shaderDataTypeToOpenGLBaseType(l.type),
				l.normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				offset);
			i++;
		}
		
		vertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer_) {
		glBindVertexArray(rendererId);
		indexBuffer_->bind();
		indexBuffer = indexBuffer_;
	}


	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &rendererId);
	}
	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &rendererId);
	}
}
