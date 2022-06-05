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
		unsigned                 rendererId;
		unsigned                 colorAttachment;
		unsigned                 depthAttachment;
		FramebufferSpecification spec;
	};
}
