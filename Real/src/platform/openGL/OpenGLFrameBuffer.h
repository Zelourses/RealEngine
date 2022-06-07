#pragma once

#include "real/renderer/Framebuffer.h"

namespace Real {
	class OpenGLFrameBuffer : public Framebuffer {
	public:
		OpenGLFrameBuffer(const FramebufferSpecification& spec);

		const FramebufferSpecification& getSpec() const override;
		void bind() override;
		void unbind() override;
		unsigned getColorAttachmentID() override;

		virtual void resize(const glm::vec2& newSize) override;

		void invalidate();

		~OpenGLFrameBuffer();

	private:
		unsigned                 rendererId = 0;
		unsigned                 colorAttachment = 0;
		unsigned                 depthAttachment = 0;
		FramebufferSpecification spec;
	};
}
