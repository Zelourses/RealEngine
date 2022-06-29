#pragma once

#include "real/renderer/Framebuffer.h"

namespace Real {
	class OpenGLFrameBuffer : public Framebuffer {
	public:
		OpenGLFrameBuffer(const FramebufferSpecification& spec);

		const FramebufferSpecification& getSpec() const override;
		void							bind() override;
		void							unbind() override;
		unsigned						getColorAttachmentID(unsigned index = 0) override;
		void							clearAttachment(size_t index, int value) override;
		virtual int						readPixel(size_t index, int x, int y) override;


		virtual void resize(const glm::vec2& newSize) override;

		void invalidate();

		~OpenGLFrameBuffer();

	private:
		unsigned				 rendererId = 0;
		FramebufferSpecification spec;

		std::vector<FramebufferTextureSpecification> colorAttachmentSpecifications;
		//There is actuallly always one format
		FramebufferTextureSpecification depthAttachemtSpecification = FramebufferTextureFormat::NONE;

		std::vector<unsigned> colorAttachments;
		unsigned			  depthAttachment = 0;
	};
}
