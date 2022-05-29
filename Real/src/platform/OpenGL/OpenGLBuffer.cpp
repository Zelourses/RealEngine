#include "repch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Real {

	////////////VertexBuffer////////////
	OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned size) { 
		RE_PROFILE_FUNCTION();

		glCreateBuffers(1, &rendererId);
        glBindBuffer(GL_ARRAY_BUFFER, rendererId);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size) {
		glCreateBuffers(1, &rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
		//a little bit bad hard-coding of draw in static mode
		glBufferData(GL_ARRAY_BUFFER,size, vertices, GL_STATIC_DRAW);
	}
	void OpenGLVertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
	}
	void OpenGLVertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//FIXME: For now this function does not request the offset in data
	void OpenGLVertexBuffer::setData(const void* data, unsigned size) {
	    glBindBuffer(GL_ARRAY_BUFFER, rendererId);
		glBufferSubData(GL_ARRAY_BUFFER, 0,size,data);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &rendererId);
	}


	////////////IndexBuffer////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, unsigned int count)
		:count(count){
		glCreateBuffers(1, &rendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
		
		//a little bit bad hard-coding of draw in static mode
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}
	void OpenGLIndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
	}
	void OpenGLIndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &rendererId);
	}


}
