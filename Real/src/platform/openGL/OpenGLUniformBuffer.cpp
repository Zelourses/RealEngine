#include "repch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Real {
	OpenGLUniformBuffer::OpenGLUniformBuffer(size_t size, unsigned binding) {
		glCreateBuffers(1, &rendererId);
		//TODO: add hints for drawing
		glNamedBufferData(rendererId, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, rendererId);
	}
	void OpenGLUniformBuffer::setData(const void* data, size_t size, size_t offset) {
		glNamedBufferSubData(rendererId, offset, size, data);
	}
	OpenGLUniformBuffer::~OpenGLUniformBuffer() {
		glDeleteBuffers(1, &rendererId);
	}
}
