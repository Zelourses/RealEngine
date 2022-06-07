#include "repch.h"
#include "OpenGLFrameBuffer.h"

#include "glad/glad.h"

namespace Real {
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FramebufferSpecification& spec)
	    : spec(spec) {
		invalidate();
	}
	const FramebufferSpecification& OpenGLFrameBuffer::getSpec() const {
		return spec;
	}
	void OpenGLFrameBuffer::invalidate() {
		if (rendererId) {
			glDeleteFramebuffers(1, &rendererId);
			glDeleteTextures(1, &colorAttachment);
			glDeleteTextures(1, &depthAttachment);
		}

		glCreateFramebuffers(1, &rendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, rendererId);

		glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
		glBindTexture(GL_TEXTURE_2D, colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec.width, spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
		glBindTexture(GL_TEXTURE_2D, depthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, spec.width, spec.height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		RE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");
	}
	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		glDeleteFramebuffers(1, &rendererId);
		glDeleteTextures(1, &colorAttachment);
		glDeleteTextures(1, &depthAttachment);
	}
	void OpenGLFrameBuffer::resize(const glm::vec2& newSize) {
		spec.width  = static_cast<int>(newSize.x);
		spec.height = static_cast<int>(newSize.y);

		invalidate();
	}
	void OpenGLFrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, rendererId);
		glViewport(0, 0, spec.width, spec.height);
	}
	void OpenGLFrameBuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	unsigned OpenGLFrameBuffer::getColorAttachmentID() {
		return colorAttachment;
	}
}