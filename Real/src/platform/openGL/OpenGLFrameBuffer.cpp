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

	static void attachColorTexture(unsigned id, unsigned samples, GLenum internalFormat, GLenum format, unsigned width, unsigned height, size_t index) {
		bool isMultisampled = samples > 1;
		if (isMultisampled) {
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

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

	static GLenum realTextureFormatToGL(Real::FramebufferTextureFormat format) {
		switch (format) {
			case Real::FramebufferTextureFormat::RGBA8: return GL_RGBA8;
			case Real::FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			default: {
				RE_CORE_ASSERT(false, "unknown format");
				return GL_FALSE;
				break;
			}
		}
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
					Utils::attachColorTexture(colorAttachments[i], spec.samples, GL_RGBA8, GL_RGBA, spec.width, spec.height, i);
					break;
				}
				case FramebufferTextureFormat::RED_INTEGER: {
					Utils::attachColorTexture(colorAttachments[i], spec.samples, GL_R32I, GL_RED_INTEGER, spec.width, spec.height, i);
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

	int OpenGLFrameBuffer::readPixel(size_t index, int x, int y) {
		RE_CORE_ASSERT(index < colorAttachments.size());

		glReadBuffer(static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + index));
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFrameBuffer::clearAttachment(size_t index, int value) {
		RE_CORE_ASSERT(index < colorAttachments.size());

		auto&& spec = colorAttachmentSpecifications[index];
		glClearTexImage(colorAttachments[index],
						0,
						Utils::realTextureFormatToGL(spec.textureFormat),
						GL_INT,
						&value);
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

	unsigned OpenGLFrameBuffer::getColorAttachmentID(unsigned index /*=0*/) {
		RE_CORE_ASSERT(index < colorAttachments.size());
		return colorAttachments[index];
	}

}