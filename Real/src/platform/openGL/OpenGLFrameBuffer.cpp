#include "repch.h"
#include "OpenGLFrameBuffer.h"

#include "glad/glad.h"

namespace Utils {
	static bool isDepthFormat(Real::FramebufferTextureFormat f) {
		switch (f) {
			case Real::FramebufferTextureFormat::DEPTH24STENCIL8:
				return true;
				break;
			default:
				return false;
				break;
		}
	}

	static inline GLenum textureTarget(bool isMultisampling) {
		return isMultisampling ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void createTextures(bool isMultisampled, unsigned* outIDs, size_t count) {
		glCreateTextures(textureTarget(isMultisampled), static_cast<GLsizei>(count), outIDs);
	}

	static void bindTexture(bool multisampled, unsigned id) {
		glBindTexture(textureTarget(multisampled), id);
	}

	static void attachColorTexture(unsigned id, unsigned samples, GLenum format, unsigned width, unsigned height, size_t index) {
		bool isMultisampled = samples > 1;
		if (isMultisampled) {
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + index), textureTarget(isMultisampled), id, GL_ZERO);
	}

	static void attachDepthTexture(unsigned id, unsigned samples, GLenum format, GLenum attachmentType, unsigned width, unsigned height) {
		bool isMultisampled = samples > 1;
		if (isMultisampled) {
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		} else {
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(isMultisampled), id, 0);
	}
}

namespace Real {
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FramebufferSpecification& spec)
		: spec(spec) {
		for (auto&& format: spec.attachments.attachments) {
			if (!Utils::isDepthFormat(format.textureFormat)) {
				colorAttachmentSpecifications.emplace_back(format);
			} else {
				depthAttachemtSpecification = format;
			}
		}

		invalidate();
	}
	const FramebufferSpecification& OpenGLFrameBuffer::getSpec() const {
		return spec;
	}
	void OpenGLFrameBuffer::invalidate() {
		if (rendererId) {
			glDeleteFramebuffers(1, &rendererId);
			glDeleteTextures(static_cast<GLsizei>(colorAttachments.size()), colorAttachments.data());
			glDeleteTextures(1, &depthAttachment);

			colorAttachments.clear();
			depthAttachment = 0;
		}

		glCreateFramebuffers(1, &rendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, rendererId);

		bool multisample = spec.samples > 1;
		//Attachments

		colorAttachments.resize(colorAttachmentSpecifications.size());
		Utils::createTextures(multisample, colorAttachments.data(), colorAttachments.size());

		for (size_t i = 0, end = colorAttachments.size(); i < end; ++i) {
			Utils::bindTexture(multisample, colorAttachments[i]);
			switch (colorAttachmentSpecifications[i].textureFormat) {
				case FramebufferTextureFormat::RGBA8: {
					Utils::attachColorTexture(colorAttachments[i], spec.samples, GL_RGBA8, spec.width, spec.height, i);
					break;
				}
				default: {
					break;
				}
			}
		}

		if (depthAttachemtSpecification.textureFormat != FramebufferTextureFormat::NONE) {
			Utils::createTextures(multisample, &depthAttachment, 1);
			Utils::bindTexture(multisample, depthAttachment);
			switch (depthAttachemtSpecification.textureFormat) {
				case FramebufferTextureFormat::DEPTH24STENCIL8: {
					Utils::attachDepthTexture(depthAttachment, spec.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, spec.width, spec.height);
					break;
				}
				default: {
					break;
				}
			}
		}
		auto colorAttacmentsSize = colorAttachments.size();
		if (colorAttacmentsSize > 1) {
			RE_CORE_ASSERT(colorAttacmentsSize <= 4);
			GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
			glDrawBuffers(static_cast<GLsizei>(colorAttacmentsSize), buffers);
		} else if (colorAttachments.empty()) {
			glDrawBuffer(GL_NONE);
		}

		RE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");
	}
	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		glDeleteFramebuffers(1, &rendererId);
		glDeleteTextures(static_cast<GLsizei>(colorAttachments.size()), colorAttachments.data());
		glDeleteTextures(1, &depthAttachment);
	}
	void OpenGLFrameBuffer::resize(const glm::vec2& newSize) {
		spec.width	= static_cast<int>(newSize.x);
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
	unsigned OpenGLFrameBuffer::getColorAttachmentID(unsigned index/*=0*/) {
		RE_CORE_ASSERT(index < colorAttachments.size());
		return colorAttachments[index];
	}
}