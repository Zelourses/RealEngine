#pragma once
#include <glm/glm.hpp>

namespace Real {

	enum class FramebufferTextureFormat {
		NONE = 0,

		//Color
		RGBA8,

		//Depth/stencil
		DEPTH24STENCIL8,

		//Defaults
		DEPTH = DEPTH24STENCIL8,

	};

	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat f) : textureFormat(f) {}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::NONE;
		//TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> atchs)
			: attachments(atchs) {}

		std::vector<FramebufferTextureSpecification> attachments;
	};

	struct FramebufferSpecification {
		unsigned width, height;
		bool	 renderInsideThisBuffer = false;
		unsigned samples				= 1;

		FramebufferAttachmentSpecification attachments;
	};

	struct Framebuffer {
		static Ref<Framebuffer> create(const FramebufferSpecification& spec);

		virtual const FramebufferSpecification& getSpec() const							 = 0;
		virtual void							bind()									 = 0;
		virtual void							unbind()								 = 0;
		virtual unsigned						getColorAttachmentID(unsigned index = 0) = 0;
		virtual void							resize(const glm::vec2& newSize)		 = 0;

		virtual ~Framebuffer() = default;
	};
}